#include "src/MediaPreparer.hpp"
#include <ui_MediaPreparer.h>

#include <src/File.hpp>
#include <src/Library.hpp>
#include <src/Settings.hpp>

#include <QCloseEvent>
#include <QFileDialog>
#include <QFuture>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QString>
#include <QSystemTrayIcon>
#include <QTime>
#include <QtConcurrent/QtConcurrent>
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
using namespace nlohmann;
using namespace std;

MediaPreparer::MediaPreparer(QWidget *parent) : QWidget(parent), ui(new Ui::MediaPreparer) {
	ui->setupUi(this);
	Init();
}

void MediaPreparer::closeEvent(QCloseEvent *event) {
	if ((!workerEncode.isRunning() && !workerScan.isRunning()) || cancel()) {
		event->accept();
	} else {
		event->ignore();
	}
}

MediaPreparer::~MediaPreparer() {
	if (diffPreset())
		savePreset("Auto Saved");
	saveConfig();
	delete ui;
}

void MediaPreparer::scanLibrary(bf::path dir) {
	emit started_workerScan();

	emit progress_updated_scan(QString("Checking Directory..."), 0);

	if (bf::exists(dir)) {

		ui->setting_dirOutput->setText(QString((dir.string() + "\\Converted").c_str()));

		emit progress_updated_scan(QString("Scanning Files..."), 0);
		log("Scanning directory: " + dir.string());

		bool recursive = ui->setting_subdirectories->isChecked();
		if (library->Init(dir, settings, recursive)) {
			for (int i = 0; i < library->size() && cancelScan != true; i++) {
				File &file = library->getFile(i);
				scanIndex = i;

				QProcess process;
				if (bf::exists("./tools/ffprobe.exe")) {
					process.setProgram(QString("./tools/ffprobe"));
				} else if (bf::exists("../tools/ffprobe.exe")) {
					process.setProgram(QString("../tools/ffprobe"));
				} else {
					process.setProgram(QString("ffprobe"));
				}
				process.setArguments(QStringList{QString("-v"), QString("quiet"), QString("-show_entries"),
												 QString("format=duration:stream=codec_type:stream=codec_name"),
												 QString("-of"), QString("json"), QString(file.pathStr().c_str())});

				emit progress_updated_scan(QString((file.nameStr() + file.extensionStr()).c_str()));
				log("Reading File: " + file.pathStr());

				process.start();
				process.waitForFinished();
				QByteArray out = process.readAllStandardOutput();
				file.loadFileInfo(json::parse(out.begin(), out.end()));

				emit item_added_scan(i);
				emit progress_updated_scan((int)(((double)i / (double)library->size()) * 1000.0));
			}
			setEncodeOptions();
			library->duration();
			library->scanEncode();
			string temp = "Encode [" + to_string(library->sizeEncode()) + "]";
			if (library->sizeEncode() != NULL)
				ui->button_encode->setText(QString(temp.c_str()));
		}
	}

	if (!cancelScan) {
		if (library->size() > 0) {
			emit progress_updated_scan(QString("Complete..."), 1000);
		} else {
			emit progress_updated_scan(QString("Invalid Directory..."), 0);
		}
	} else {
		emit progress_updated_scan(QString("Canceled..."), 0);
	}

	emit finished_workerScan();
}

void MediaPreparer::encodeLibrary() {
	emit started_workerEncode();

	for (int i = 0; i < library->getFilesEncode().size() && cancelEncode != true; i++) {
		File &file = library->getFileEncode(i);
		encodeIndex = i;

		emit progress_updated_scan(QString(file.nameStr().c_str()),
								   ((double)i / (double)library->sizeEncode()) * 1000.0);
		emit progress_updated_encode(QString("Encoding File"), 0);
		emit item_changed_encode(i);

		QStringList params;
		if (bf::exists((settings->outputDir.string() + "\\" + file.nameStr() + "." + settings->container).c_str())) {
			params.push_back("-y");
		}
		params.push_back(QString("-v"));
		params.push_back(QString("quiet"));
		params.push_back(QString("-stats"));
		params.push_back(QString("-hwaccel"));
		params.push_back(QString("dxva2"));
		params.push_back(QString("-threads"));
		params.push_back(QString(settings->threads.c_str()));
		params.push_back(QString("-i"));
		params.push_back(QString(file.pathStr().c_str()));
		if (file.subtitles() == 1 && settings->subtitles.compare("Embed") == 0) {
			params.push_back(QString("-i"));
			params.push_back(QString(file.pathSubStr().c_str()));
		}
		params.push_back(QString("-map"));
		params.push_back(QString("0:0"));
		params.push_back(QString("-map"));
		params.push_back(QString("0:1?"));
		if (file.subtitles() == 1 && settings->subtitles.compare("Embed") == 0) {
			params.push_back(QString("-map"));
			params.push_back(QString("1:0"));
		}
		if (file.subtitles() > 0 && settings->subtitles.compare("Remove") != 0) {
			params.push_back(QString("-map"));
			params.push_back(QString("0:2?"));
			params.push_back(QString("-c:s"));
			params.push_back(QString("srt"));
			params.push_back(QString("-metadata:s:s:0"));
			params.push_back(QString("language=eng"));
			params.push_back(QString("-disposition:s:0"));
			params.push_back(QString("default"));
		}
		params.push_back(QString("-c:v"));
		params.push_back(QString(settings->vCodec.c_str()));
		params.push_back(QString("-crf"));
		params.push_back(QString(settings->vQuality.c_str()));
		params.push_back(QString("-c:a"));
		params.push_back(QString(settings->aCodec.c_str()));
		params.push_back(QString("-b:a"));
		params.push_back(QString((settings->aQuality + "k").c_str()));
		if (!settings->extraParams.empty()) {
			char s[2048];
			strcpy(s, settings->extraParams.c_str());
			for (char *p = strtok(s, " "); p != NULL; p = strtok(NULL, " ")) {
				params.push_back(p);
			}
		}
		params.push_back(QString("-metadata"));
		params.push_back(QString(("title=" + file.nameStr()).c_str()));
		params.push_back(QString("-metadata"));
		params.push_back(QString("comment=Processed by SuperEpicFuntime Media Preparer (Reborn)"));
		params.push_back(QString("-strict"));
		params.push_back(QString("-2"));
		params.push_back(
			QString((settings->tempDir.string() + "\\" + file.nameStr() + "." + settings->container).c_str()));

		QProcess process;
		if (bf::exists("./tools/ffmpeg.exe")) {
			process.setProgram(QString("./tools/ffmpeg"));
		} else if (bf::exists("../tools/ffmpeg.exe")) {
			process.setProgram(QString("../tools/ffmpeg"));
		} else {
			process.setProgram(QString("ffmpeg"));
		}
		process.setArguments(params);

		process.start();
		process.waitForFinished(-1);
		if (!cancelEncode) {
			if (bf::exists(settings->tempDir.string() + "\\" + file.nameStr() + "." + settings->container)) {
				bf::copy_file(settings->tempDir.string() + "\\" + file.nameStr() + "." + settings->container,
							  settings->outputDir.string() + "\\" + file.nameStr() + "." + settings->container);
				if (bf::exists(settings->outputDir.string() + "\\" + file.nameStr() + "." + settings->container)) {
					bf::remove(settings->tempDir.string() + "\\" + file.nameStr() + "." + settings->container);
				}
			}
		}
		emit progress_updated_scan((int)(((double)i / (double)library->sizeEncode()) * 1000.0));
	}
	if (!cancelEncode) {
		emit progress_updated_scan("Complete...", 1000);
	} else {
		emit progress_updated_scan("Canceled...", 0);
	}

	emit finished_workerEncode();
}

void MediaPreparer::runWorkerScan() {
	if (!workerScan.isRunning() && workerScan.isFinished()) {
		workerScan = QtConcurrent::run(this, &MediaPreparer::scanLibrary, getPath());
	}
}

void MediaPreparer::runWorkerEncode() {
	if (!workerEncode.isRunning() && workerEncode.isFinished()) {
		workerEncode = QtConcurrent::run(this, &MediaPreparer::encodeLibrary);
	}
}
/*
void MediaPreparer::updateEncodeList() {}

void MediaPreparer::toggleEncodeFile(int position, int null) {
	File f = library->getFile(position);
	if (ui->list_Library->item(position, 0)->checkState() == Qt::Checked) {
		cout << "Force Encoding: " << f.nameStr();
		library->forceEncode(f);
	} else {
		cout << "Skipping File: " << f.nameStr();
		library->skipEncode(f);
	}
}
*/

void MediaPreparer::loadConfig() {
	blockAllSignals(true);
	ui->setting_vCodec->clear();
	ui->setting_aCodec->clear();
	ui->setting_container->clear();
	ui->setting_preset->clear();

	ui->setting_directory->setText(QString(settings->libraryDir.string().c_str()));
	ui->setting_dirOutput->setText(QString(settings->outputDir.string().c_str()));

	for (int i = 0; i < settings->vCodecList.size(); i++) {
		ui->setting_vCodec->addItem(QString(settings->vCodecList[i][0].c_str()));
	}
	for (int i = 0; i < settings->aCodecList.size(); i++) {
		ui->setting_aCodec->addItem(QString(settings->aCodecList[i][0].c_str()));
	}
	for (int i = 0; i < settings->containerList.size(); i++) {
		ui->setting_container->addItem(QString(settings->containerList[i].c_str()));
	}
	for (int i = 0; i < settings->presetNameList.size(); i++) {
		ui->setting_preset->addItem(QString(settings->presetNameList[i].c_str()));
	}
	blockAllSignals(false);
}

bool MediaPreparer::checkConfig() {
	if (!bf::exists(ui->setting_directory->text().toStdString().c_str()))
		return false;
	if (!(library->size() > 0))
		return false;
	if (!bf::create_directories(ui->setting_dirOutput->text().toStdString().c_str()) &&
		!bf::exists(ui->setting_dirOutput->text().toStdString().c_str())) {
		if (bf::is_empty(ui->setting_dirOutput->text().toStdString().c_str()))
			bf::remove(ui->setting_dirOutput->text().toStdString().c_str());
		return false;
	}
	return true;
}

void MediaPreparer::saveConfig() {
	settings->saveConfig();
}

void MediaPreparer::loadPreset(string preset) {
	blockAllSignals(true);
	settings->loadPreset(preset);
	ui->setting_preset->setCurrentText(QString(settings->presetName.c_str()));
	ui->setting_preset->setToolTip(QString(settings->presetPath.string().c_str()));
	ui->setting_vCodec->setCurrentText(QString(settings->vCodec.c_str()));
	ui->setting_aCodec->setCurrentText(QString(settings->aCodec.c_str()));
	ui->setting_vQuality->setValue(stoi(settings->vQuality.c_str()));
	ui->setting_aQuality->setValue(stoi(settings->aQuality.c_str()));
	ui->setting_container->setCurrentText(QString(settings->container.c_str()));
	ui->setting_subtitles->setCurrentText(QString(settings->subtitles.c_str()));
	ui->setting_threads->setText(QString(settings->threads.c_str()));
	ui->setting_extraParams->setText(QString(settings->extraParams.c_str()));
	blockAllSignals(false);
	setEncodeOptions();
}

void MediaPreparer::loadPreset(QString preset) {
	loadPreset(preset.toStdString());
}

bool MediaPreparer::diffPreset() {
	if (ui->setting_vCodec->currentText().toStdString().compare(settings->vCodec) != 0)
		return true;
	if (ui->setting_aCodec->currentText().toStdString().compare(settings->aCodec) != 0)
		return true;
	if (ui->setting_vQuality->value() != stoi(settings->vQuality))
		return true;
	if (ui->setting_aQuality->value() != stoi(settings->aQuality))
		return true;
	if (ui->setting_container->currentText().toStdString().compare(settings->container) != 0)
		return true;
	if (ui->setting_subtitles->currentText().toStdString().compare(settings->subtitles) != 0)
		return true;
	if (ui->setting_threads->text().toStdString().compare(settings->threads) != 0)
		return true;
	if (ui->setting_extraParams->text().toStdString().compare(settings->extraParams) != 0)
		return true;
	return false;
}

void MediaPreparer::savePreset(std::string name) {
	setEncodeOptions();
	settings->savePresetAs(name);
	loadPreset(settings->presetName);
	emit progress_updated_scan(QString(("Saved Preset: " + name).c_str()));
}

void MediaPreparer::savePreset() {
	savePreset("Custom");
}

void MediaPreparer::setEncodeOptions() {
	settings->vCodec = ba::trim_copy(ui->setting_vCodec->currentText().toStdString());
	settings->aCodec = ba::trim_copy(ui->setting_aCodec->currentText().toStdString());
	settings->vQuality = ba::trim_copy(to_string(ui->setting_vQuality->value()));
	settings->aQuality = ba::trim_copy(to_string(ui->setting_aQuality->value()));
	settings->container = ba::trim_copy(ui->setting_container->currentText().toStdString());
	settings->subtitles = ba::trim_copy(ui->setting_subtitles->currentText().toStdString());
	settings->outputDir = ba::trim_copy(ui->setting_dirOutput->text().toStdString());
	settings->threads = ba::trim_copy(ui->setting_threads->text().toStdString());
	settings->extraParams = ba::trim_copy(ui->setting_extraParams->text().toStdString());

	ui->button_encode->setText(QString(("Encode [" + to_string(library->sizeEncode()) + "]").c_str()));
}

bf::path MediaPreparer::getPath(string path) {
	ba::replace_all(path, "/", "\\");
	ui->setting_directory->setText(QString(path.c_str()));
	return bf::path(path);
}

bf::path MediaPreparer::getPath() {
	return getPath(ui->setting_directory->text().toStdString());
}

void MediaPreparer::lockUILoad(bool b) {
	bool toggle;
	if (b) {
		toggle = false;
	} else {
		toggle = true;
	}
	if (b || checkConfig())
		ui->button_encode->setEnabled(toggle);
	ui->button_browse_directory->setEnabled(toggle);
	ui->button_scan_directory->setEnabled(toggle);
	ui->setting_directory->setEnabled(toggle);
	encodeTab(false);
	ui->progress_worker->setMaximum(1000);
	if (b) {
		ui->button_encode->setText(QString("Cancel"));
		ui->button_encode->setEnabled(true);
	} else {
		ui->button_encode->setText(QString(("Encode [" + to_string(library->sizeEncode()) + "]").c_str()));
		ui->button_encode->setEnabled(true);
	}
}

void MediaPreparer::lockUIEncode(bool b) {
	bool toggle;
	if (b) {
		toggle = false;
	} else {
		toggle = true;
	}
	ui->button_browse_directory->setEnabled(toggle);
	ui->button_scan_directory->setEnabled(toggle);
	ui->setting_directory->setEnabled(toggle);
	ui->button_browse_dirOutput->setEnabled(toggle);
	ui->setting_directory->setEnabled(toggle);
	ui->setting_vCodec->setEnabled(toggle);
	ui->setting_aCodec->setEnabled(toggle);
	ui->setting_vQuality->setEnabled(toggle);
	ui->setting_aQuality->setEnabled(toggle);
	ui->setting_container->setEnabled(toggle);
	ui->setting_directory->setEnabled(toggle);
	ui->setting_dirOutput->setEnabled(toggle);
	ui->setting_subtitles->setEnabled(toggle);
	ui->setting_threads->setEnabled(toggle);
	ui->setting_extraParams->setEnabled(toggle);
	ui->setting_preset->setEnabled(toggle);
	if (b) {
		ui->button_encode->setText(QString("Cancel"));
		ui->button_encode->setEnabled(true);
	} else {
		ui->button_encode->setText(QString(("Encode [" + to_string(library->sizeEncode()) + "]").c_str()));
		ui->button_encode->setEnabled(true);
	}
	encodeTab(true);
	ui->progress_worker->setMaximum(library->duration());
}

void MediaPreparer::updateGUI() {
	if (workerEncode.isRunning()) {
		if (workerTimeStamp.isValid()) {
			ui->value_encode_runtime->setText(
				QString("%1:%2:%3")
					.arg(workerTimeStamp.elapsed() / 3600000, 2, 10, QChar('0'))
					.arg((workerTimeStamp.elapsed() % 3600000) / 60000, 2, 10, QChar('0'))
					.arg(((workerTimeStamp.elapsed() % 3600000) % 60000) / 1000, 2, 10, QChar('0')));
		}
		if (fileTimeStamp.isValid()) {
			ui->progress_encode->setFormat(
				QString("%1:%2:%3")
					.arg(fileTimeStamp.elapsed() / 3600000, 2, 10, QChar('0'))
					.arg((fileTimeStamp.elapsed() % 3600000) / 60000, 2, 10, QChar('0'))
					.arg(((fileTimeStamp.elapsed() % 3600000) % 60000) / 1000, 2, 10, QChar('0')));
			ui->progress_encode->setValue(fileTimeStamp.elapsed());
			ui->progress_encode->repaint();
		}
	}
}

void MediaPreparer::encodeTab(bool b) {
	if (b) {
		ui->container_settings_tabs->addTab(containerEncode, QString("Encode"));
		ui->container_settings_tabs->addTab(containerEncodeList, QString("Queue"));
		ui->value_encode_vCodec->setText(QString(settings->vCodec.c_str()));
		ui->value_encode_aCodec->setText(QString(settings->aCodec.c_str()));
		ui->value_encode_vQuality->setText(QString(settings->vQuality.c_str()));
		ui->value_encode_aQuality->setText(QString((settings->aQuality + "k").c_str()));
		ui->value_encode_subtitles->setText(QString(settings->subtitles.c_str()));
		ui->value_encode_container->setText(QString(("." + settings->container).c_str()));
		ui->list_encode_Library->setRowCount(library->sizeEncode());
		for (int i = 0; i < library->sizeEncode(); i++) {
			File &item = library->getFileEncode(i);
			ui->list_encode_Library->setItem(i, 0, new QTableWidgetItem(QString(item.nameStr().c_str())));
			ui->list_encode_Library->setItem(i, 1, new QTableWidgetItem(QString(item.vcodec().c_str())));
			ui->list_encode_Library->setItem(i, 2, new QTableWidgetItem(QString(item.acodec().c_str())));
			ui->list_encode_Library->setItem(i, 3, new QTableWidgetItem(QString(item.extensionStr().c_str())));
			ui->list_encode_Library->setItem(i, 4, new QTableWidgetItem(QString(item.subtitlesStr().c_str())));
		}
		ui->container_settings_tabs->setCurrentWidget(ui->container_encode);
	} else {
		containerEncode = ui->container_encode;
		containerEncodeList = ui->container_encode_fileList;
		ui->container_settings_tabs->removeTab(4);
		ui->container_settings_tabs->removeTab(3);
	}
}

void MediaPreparer::Init() {
	createTrayIcon();
	InitGUI();

	connect(this, SIGNAL(started_workerScan()), this, SLOT(workerScanStart()));
	connect(this, SIGNAL(item_added_scan(int)), this, SLOT(workerScanAddItem(int)));
	connect(this, SIGNAL(finished_workerScan()), this, SLOT(workerScanEnd()));

	connect(this, SIGNAL(started_workerEncode()), this, SLOT(workerEncodeStart()));
	connect(this, SIGNAL(item_changed_encode(int)), this, SLOT(workerEncodeChangeItem(int)));
	connect(this, SIGNAL(finished_workerEncode()), this, SLOT(workerEncodeEnd()));

	connect(this, SIGNAL(progress_updated_scan(QString, int)), this, SLOT(workerScanUpdateProgress(QString, int)));
	connect(this, SIGNAL(progress_updated_scan(QString)), this, SLOT(workerScanUpdateProgress(QString)));
	connect(this, SIGNAL(progress_updated_scan(int)), this, SLOT(workerScanUpdateProgress(int)));

	connect(this, SIGNAL(progress_updated_encode(QString, int)), this, SLOT(workerEncodeUpdateProgress(QString, int)));
	connect(this, SIGNAL(progress_updated_encode(QString)), this, SLOT(workerEncodeUpdateProgress(QString)));
	connect(this, SIGNAL(progress_updated_encode(int)), this, SLOT(workerEncodeUpdateProgress(int)));

	connect(ui->button_savePreset, SIGNAL(clicked()), this, SLOT(savePreset()));

	connect(ui->setting_preset, SIGNAL(currentTextChanged(const QString)), this, SLOT(loadPreset(QString)));

	connect(ui->button_scan_directory, SIGNAL(clicked()), this, SLOT(runWorkerScan()));
	connect(ui->setting_directory, SIGNAL(returnPressed()), this, SLOT(runWorkerScan()));

	connect(ui->button_browse_directory, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(ui->button_browse_directory, 0);
	connect(ui->button_browse_dirOutput, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(ui->button_browse_dirOutput, 1);
	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(browseDialog(int)));

	// connect(ui->list_Library, SIGNAL(cellChanged(int, int)), this, SLOT(toggleEncodeFile(int, int)));

	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateGUI()));
	updateTimer->start(100);

	updateGUI();

	encodeTab(false);
	ui->container_settings_tabs->setCurrentWidget(ui->container_settings_basic);

	if (!settings->preserveLog)
		bf::remove(settings->baseDir.string() + "\\log.txt");

	showTrayMessage("Loaded");
}

void MediaPreparer::InitGUI() {
	blockAllSignals(true);
	ui->setting_vCodec->clear();
	ui->setting_aCodec->clear();
	ui->setting_container->clear();
	ui->setting_preset->clear();

	ui->setting_directory->setText(QString(settings->libraryDir.string().c_str()));
	ui->setting_dirOutput->setText(QString(settings->outputDir.string().c_str()));

	for (int i = 0; i < settings->vCodecList.size(); i++) {
		ui->setting_vCodec->addItem(QString(settings->vCodecList[i][0].c_str()));
	}
	for (int i = 0; i < settings->aCodecList.size(); i++) {
		ui->setting_aCodec->addItem(QString(settings->aCodecList[i][0].c_str()));
	}
	for (int i = 0; i < settings->containerList.size(); i++) {
		ui->setting_container->addItem(QString(settings->containerList[i].c_str()));
	}
	for (int i = 0; i < settings->presetPathList.size(); i++) {
		ui->setting_preset->addItem(
			QString(settings->presetPathList[i].filename().replace_extension().string().c_str()));
	}

	ui->setting_preset->setCurrentText(QString(settings->presetName.c_str()));
	ui->setting_preset->setToolTip(QString(settings->presetPath.string().c_str()));
	ui->setting_vCodec->setCurrentText(QString(settings->vCodec.c_str()));
	ui->setting_aCodec->setCurrentText(QString(settings->aCodec.c_str()));
	ui->setting_vQuality->setValue(stoi(settings->vQuality.c_str()));
	ui->setting_aQuality->setValue(stoi(settings->aQuality.c_str()));
	ui->setting_container->setCurrentText(QString(settings->container.c_str()));
	ui->setting_subtitles->setCurrentText(QString(settings->subtitles.c_str()));
	ui->setting_threads->setText(QString(settings->threads.c_str()));
	ui->setting_extraParams->setText(QString(settings->extraParams.c_str()));

	/*
	ui->list_Library->setColumnWidth(0, 20);
	ui->list_Library->setColumnWidth(1, 155);
	ui->list_Library->setColumnWidth(2, 65);
	ui->list_Library->setColumnWidth(3, 65);
	ui->list_Library->setColumnWidth(4, 65);
	ui->list_Library->setColumnWidth(5, 65);
	*/

	ui->list_Library->setColumnWidth(0, 155);
	ui->list_Library->setColumnWidth(1, 70);
	ui->list_Library->setColumnWidth(2, 70);
	ui->list_Library->setColumnWidth(3, 70);
	ui->list_Library->setColumnWidth(4, 70);

	blockAllSignals(false);
}

void MediaPreparer::log(string msg) {
	bf::fstream fs;
	fs.open(settings->logPath, bf::fstream::in | bf::fstream::out | bf::fstream::app);

	char timeStamp[1024];
	time_t t = time(0);
	struct tm *now = localtime(&t);
	strftime(timeStamp, sizeof(timeStamp), "%F %T", now);

	fs << "[" << timeStamp << "] " << msg << endl;
}
void MediaPreparer::log(QString msg) {
	log(msg.toStdString());
}

void MediaPreparer::createTrayActions() {
	minimizeAction = new QAction(tr("Mi&nimize"), this);
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

	restoreAction = new QAction(tr("&Restore"), this);
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MediaPreparer::createTrayIcon() {
	createTrayActions();
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
	trayIcon->setToolTip(tr("SEF Media Preparer"));
	trayIcon->setIcon(QIcon(":seflogo.png"));
	trayIcon->show();
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
			SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
}

void MediaPreparer::showTrayMessage(QString msg) {
	QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
	trayIcon->showMessage(tr("SEF Media Preparer"), msg, icon, 10000);
}

void MediaPreparer::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		trayIcon->show();
		break;
	case QSystemTrayIcon::DoubleClick:
		show();
		break;
	case QSystemTrayIcon::MiddleClick:
		showTrayMessage("swag");
		break;
	default:;
	}
}

void MediaPreparer::browseDialog(int type) {
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setDirectory(QDir(QString(ui->setting_directory->text())));
	dialog.exec();
	if (dialog.result() == QDialog::Accepted) {
		QStringList dir = dialog.selectedFiles();
		switch (type) {
		case 0:
			ui->setting_directory->setText(dir[0]);
			runWorkerScan();
			break;
		case 1:
			ui->setting_dirOutput->setText(dir[0]);
			break;
		default:
			break;
		}
	}
}

bool MediaPreparer::cancelDialog() {
	QMessageBox confirm;
	confirm.setText("Are you sure?");
	confirm.setInformativeText("Any unfinished progress will be lost.");
	confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	confirm.setDefaultButton(QMessageBox::No);
	int opt = confirm.exec();
	if (opt == QMessageBox::No) {
		return false;
	}
	return true;
}

bool MediaPreparer::cancel() {
	if (!cancelDialog())
		return false;
	if (workerScan.isRunning()) {
		cancelScan = true;
		QProcess kill;
		kill.start(QString("taskkill /F /T /IM ffprobe.exe"));
		kill.waitForFinished();
		workerScan.waitForFinished();
	}
	if (workerEncode.isRunning()) {
		cancelEncode = true;
		QProcess kill;
		kill.start(QString("taskkill /F /T /IM ffmpeg.exe"));
		kill.waitForFinished();
		workerEncode.waitForFinished();
		if (bf::exists(settings->tempDir.string() + "\\" + library->getFileEncode(encodeIndex).nameStr() + "." +
					   settings->container)) {
			bf::remove(settings->tempDir.string() + "\\" + library->getFileEncode(encodeIndex).nameStr() + "." +
					   settings->container);
		}
	}
}

void MediaPreparer::blockAllSignals(bool b) {
	QList<QWidget *> widgets = ui->container_main->findChildren<QWidget *>();
	foreach (QWidget *x, widgets) { x->blockSignals(b); }
}

void MediaPreparer::workerScanStart() {
	ui->list_Library->clearContents();
	lockUILoad(true);
}

void MediaPreparer::workerScanAddItem(int pos) {
	blockAllSignals(true);
	File item = library->getFile(pos);
	ui->label_fileCount->setText(
		QString(("<html><head/><body><p>" + std::to_string(pos + 1) + " file(s) found</p></body></html>").c_str()));

	ui->list_Library->setRowCount(pos + 1);
	// ui->list_Library->setItem(pos, 0, new QTableWidgetItem());
	// ui->list_Library->item(pos, 0)->setCheckState(Qt::Checked);
	ui->list_Library->setItem(pos, 0, new QTableWidgetItem(QString(item.nameStr().c_str())));
	ui->list_Library->setItem(pos, 1, new QTableWidgetItem(QString(item.vcodec().c_str())));
	ui->list_Library->setItem(pos, 2, new QTableWidgetItem(QString(item.acodec().c_str())));
	ui->list_Library->setItem(pos, 3, new QTableWidgetItem(QString(item.extensionStr().c_str())));
	ui->list_Library->setItem(pos, 4, new QTableWidgetItem(QString(item.subtitlesStr().c_str())));
	blockAllSignals(false);
}

void MediaPreparer::workerScanEnd() {
	lockUILoad(false);
	activateWindow();
}

void MediaPreparer::on_button_encode_clicked() {
	if (workerEncode.isRunning()) {
		cancelEncode = true;
		cancel();
	} else if (workerScan.isRunning()) {
		cancelScan = true;
		cancel();
	} else {
		setEncodeOptions();
		cancelScan = false;
		cancelEncode = false;
		runWorkerEncode();
	}
}

void MediaPreparer::workerEncodeStart() {

	lockUIEncode(true);
	workerTimeStamp.start();
}

void MediaPreparer::workerEncodeChangeItem(int pos) {
	blockAllSignals(true);
	if (pos > 0) {
		ui->value_encode_lastFile->setText(
			QString("%1:%2:%3")
				.arg(fileTimeStamp.elapsed() / 3600000, 2, 10, QChar('0'))
				.arg((fileTimeStamp.elapsed() % 3600000) / 60000, 2, 10, QChar('0'))
				.arg(((fileTimeStamp.elapsed() % 3600000) % 60000) / 1000, 2, 10, QChar('0')));
	}
	File f = library->getFileEncode(pos);
	ui->value_encode_count->setText(QString((to_string(pos + 1) + " / " + to_string(library->sizeEncode())).c_str()));
	ui->value_encode_file->setText(QString(f.nameStr().c_str()));
	ui->value_encode_file_vCodec->setText(QString(f.vcodec().c_str()));
	ui->value_encode_file_aCodec->setText(QString(f.acodec().c_str()));
	ui->value_encode_file_container->setText(QString(f.extensionStr().c_str()));
	ui->value_encode_file_subtitles->setText(QString(f.subtitlesStr().c_str()));
	ui->list_encode_Library->removeRow(0);
	ui->value_encode_file_duration->setText(QString("%1:%2:%3")
												.arg(f.duration() / 3600000, 2, 10, QChar('0'))
												.arg((f.duration() % 3600000) / 60000, 2, 10, QChar('0'))
												.arg(((f.duration() % 3600000) % 60000) / 1000, 2, 10, QChar('0')));
	ui->progress_encode->setMaximum(f.duration());
	fileTimeStamp.restart();
	blockAllSignals(false);
}

void MediaPreparer::workerEncodeEnd() {
	lockUIEncode(false);
	activateWindow();
}

void MediaPreparer::workerScanUpdateProgress(QString msg, int percent) {
	ui->progress_worker->setFormat(msg);
	ui->progress_worker->setValue(percent);
}

void MediaPreparer::workerScanUpdateProgress(QString msg) {
	ui->progress_worker->setFormat(msg);
}

void MediaPreparer::workerScanUpdateProgress(int percent) {
	ui->progress_worker->setValue(percent);
}

void MediaPreparer::workerEncodeUpdateProgress(QString msg, int percent) {
	ui->progress_encode->setFormat(msg);
	ui->progress_worker->setValue(percent);
}

void MediaPreparer::workerEncodeUpdateProgress(QString msg) {
	ui->progress_encode->setFormat(msg);
}

void MediaPreparer::workerEncodeUpdateProgress(int percent) {
	ui->progress_worker->setValue(percent);
}
