#include "src/MediaPreparer.hpp"
#include <ui_MediaPreparer.h>

#include <src/File.hpp>
#include <src/Library.hpp>
#include <src/Settings.hpp>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QFuture>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtGui/QCloseEvent>
#include <QtWebChannel/QtWebChannel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSystemTrayIcon>
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <rapidjson/document.h>
#include <string>

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
using namespace rapidjson;
using namespace std;

namespace SuperEpicFuntime {

#ifndef SEFMediaPreparer_Section_Init

MediaPreparer::MediaPreparer(QWidget *parent) : QWidget(parent), ui(new Ui::MediaPreparer) {
	ui->setupUi(this);
	Init();
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

	connect(ui->button_savePreset, SIGNAL(clicked()), this, SLOT(saveDialog()));

	connect(ui->setting_preset, SIGNAL(currentTextChanged(const QString)), this, SLOT(loadPreset(QString)));

	connect(ui->button_scan_directory, SIGNAL(clicked()), this, SLOT(runWorkerScan()));
	connect(ui->setting_directory, SIGNAL(returnPressed()), this, SLOT(runWorkerScan()));

	connect(ui->button_browse_directory, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(ui->button_browse_directory, 0);
	connect(ui->button_browse_dirOutput, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(ui->button_browse_dirOutput, 1);
	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(browseDialog(int)));

	// connect(ui->list_Library, SIGNAL(cellChanged(int, int)), this, SLOT(toggleEncodeFile(int, int)));

	connect(ui->setting_vCodec, SIGNAL(currentIndexChanged(int)), this, SLOT(setEncodeOptions()));
	connect(ui->setting_aCodec, SIGNAL(currentIndexChanged(int)), this, SLOT(setEncodeOptions()));
	connect(ui->setting_container, SIGNAL(currentIndexChanged(int)), this, SLOT(setEncodeOptions()));

	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateGUI()));
	updateTimer->start(100);

	updateGUI();

	encodeTab(false);
	ui->container_settings_tabs->setCurrentWidget(ui->container_settings_basic);

	if (!settings->preserveLog) {
		bf::remove(settings->baseDir + "\\log.txt");
	}

	showTrayMessage("Loaded");
}

void MediaPreparer::InitGUI() {
	blockAllSignals(true);

	this->setWindowTitle(productName.c_str());

	ui->setting_vCodec->clear();
	ui->setting_aCodec->clear();
	ui->setting_container->clear();
	ui->setting_preset->clear();

	ui->setting_directory->setText(QString(settings->libraryDir.c_str()));
	ui->setting_dirOutput->setText(QString(settings->outputDir.c_str()));

	for (int i = 0; i < (int)settings->vCodecList.size(); i++) {
		ui->setting_vCodec->addItem(QString(settings->vCodecList[i][0].c_str()));
	}
	for (int i = 0; i < (int)settings->aCodecList.size(); i++) {
		ui->setting_aCodec->addItem(QString(settings->aCodecList[i][0].c_str()));
	}
	for (int i = 0; i < (int)settings->containerList.size(); i++) {
		ui->setting_container->addItem(QString(settings->containerList[i].c_str()));
	}
	for (int i = 0; i < (int)settings->presetNameList.size(); i++) {
		ui->setting_preset->addItem(QString(settings->presetNameList[i].c_str()));
	}

	ui->setting_preset->setCurrentText(QString(settings->presetName.c_str()));
	ui->setting_preset->setToolTip(QString(settings->presetPath.c_str()));
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

void MediaPreparer::closeEvent(QCloseEvent *event) {
	if ((!workerEncode.isRunning() && !workerScan.isRunning()) || cancel()) {
		event->accept();
	} else {
		event->ignore();
	}
}

MediaPreparer::~MediaPreparer() {
	saveConfig();
	delete ui;
}

#endif

#ifndef SEFMediaPreparer_Section_Config_Presets

void MediaPreparer::loadConfig() {
	blockAllSignals(true);
	ui->setting_vCodec->clear();
	ui->setting_aCodec->clear();
	ui->setting_container->clear();
	ui->setting_preset->clear();

	ui->setting_directory->setText(QString(settings->libraryDir.c_str()));
	ui->setting_dirOutput->setText(QString(settings->outputDir.c_str()));

	for (int i = 0; i < (int)settings->vCodecList.size(); i++) {
		ui->setting_vCodec->addItem(QString(settings->vCodecList[i][0].c_str()));
	}
	for (int i = 0; i < (int)settings->aCodecList.size(); i++) {
		ui->setting_aCodec->addItem(QString(settings->aCodecList[i][0].c_str()));
	}
	for (int i = 0; i < (int)settings->containerList.size(); i++) {
		ui->setting_container->addItem(QString(settings->containerList[i].c_str()));
	}
	for (int i = 0; i < (int)settings->presetNameList.size(); i++) {
		ui->setting_preset->addItem(QString(settings->presetNameList[i].c_str()));
	}
	blockAllSignals(false);
}

bool MediaPreparer::checkConfig() {
	if (!bf::exists(ui->setting_directory->text().toStdString().c_str())) {
		return false;
	}
	if (!(library->size() > 0)) {
		return false;
	}
	if (!bf::exists(ui->setting_dirOutput->text().toStdString().c_str()) &&
		!bf::create_directories(ui->setting_dirOutput->text().toStdString().c_str())) {
		if (bf::is_empty(ui->setting_dirOutput->text().toStdString().c_str())) {
			bf::remove(ui->setting_dirOutput->text().toStdString().c_str());
		}
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
	ui->setting_preset->setToolTip(QString(settings->presetPath.c_str()));
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
	loadPreset(settings->presetPath);
	InitGUI();
	emit progress_updated_scan(QString(("Saved Preset: " + settings->presetName).c_str()));
}

void MediaPreparer::savePreset() {
	savePreset("Custom");
}

#endif

#ifndef SEFMediaPreparer_Section_UI_Control

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
		ui->button_encode->setText(QString(
			("Encode [" + to_string(library->sizeEncode()) + "]")
				.c_str())); // @suppress("Function cannot be resolved") // @suppress("Method cannot be resolved")
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
			ui->list_encode_Library->setItem(i, 0, new QTableWidgetItem(QString(item.name().c_str())));
			ui->list_encode_Library->setItem(i, 1, new QTableWidgetItem(QString(item.vcodec().c_str())));
			ui->list_encode_Library->setItem(i, 2, new QTableWidgetItem(QString(item.acodec().c_str())));
			ui->list_encode_Library->setItem(i, 3, new QTableWidgetItem(QString(item.extension().c_str())));
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

#endif

#ifndef SEFMediaPreparer_Section_Tray_Icon

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

#endif

#ifndef SEFMediaPreparer_Section_Dialogs

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

void MediaPreparer::saveDialog() {
	// TODO change to custom dialog
	QString fileName =
		QFileDialog::getSaveFileName(this, "Save Preset - Will only be loaded from preset directory",
									 (settings->baseDir + "//presets//Custom").c_str(), "SEF Preset (*.preset)");
	if (!fileName.isEmpty()) {
		savePreset(fileName.toStdString());
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

#endif

#ifndef SEFMediaPreparer_Section_Utility

void MediaPreparer::setEncodeOptions() {
	settings->vCodec = ba::trim_copy(ui->setting_vCodec->currentText().toStdString());
	settings->aCodec = ba::trim_copy(ui->setting_aCodec->currentText().toStdString());
	settings->vQuality = ba::trim_copy(std::to_string(ui->setting_vQuality->value()));
	settings->aQuality = ba::trim_copy(std::to_string(ui->setting_aQuality->value()));
	settings->container = ba::trim_copy(ui->setting_container->currentText().toStdString());
	settings->subtitles = ba::trim_copy(ui->setting_subtitles->currentText().toStdString());
	settings->outputDir = ba::trim_copy(ui->setting_dirOutput->text().toStdString());
	settings->threads = ba::trim_copy(ui->setting_threads->text().toStdString());
	settings->extraParams = ba::trim_copy(ui->setting_extraParams->text().toStdString());

	if (library->isValid()) {
		library->scanEncode();
		ui->button_encode->setText(QString(("Encode [" + std::to_string(library->sizeEncode()) + "]").c_str()));
	} else {
		ui->button_encode->setText(QString("Encode [0]"));
	}
}

std::string MediaPreparer::getPath() {
	string p = ui->setting_directory->text().toStdString();
	ba::replace_all(p, "/", "\\");
	ui->setting_directory->setText(QString(p.c_str()));
	return p;
}

void MediaPreparer::log(std::string msg, int logLevel) {

	bf::fstream fs;
	fs.open(settings->logPath, bf::fstream::in | bf::fstream::out | bf::fstream::app);

	time_t rt = time(0);
	struct tm *now = localtime(&rt);

	std::string logLevelStr;
	switch (logLevel) {
	case 3:
		logLevelStr = "SEVERE";
		break;
	case 2:
		logLevelStr = "ERROR";
		break;
	case 1:
		logLevelStr = "WARNING";
		break;
	case 0:
		logLevelStr = "INFO";
		break;
	default:
		logLevelStr = "NULL";
	};

	fs << "[" << (now->tm_year + 1900) << "-" << (now->tm_mon + 1) << "-" << now->tm_mday << " " << now->tm_hour << ":"
	   << now->tm_min << ":" << now->tm_sec << "] [" << logLevelStr << "] " << msg << endl;
}

void MediaPreparer::log(QString msg, int logLevel) {
	log(msg.toStdString(), logLevel);
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
		if (bf::exists(settings->tempDir + "\\" + library->getFileEncode(encodeIndex).name() + "." +
					   settings->container)) {
			bf::remove(settings->tempDir + "\\" + library->getFileEncode(encodeIndex).name() + "." +
					   settings->container);
		}
	}
	return cancelEncode;
}

void MediaPreparer::blockAllSignals(bool b) {
	QList<QWidget *> widgets = ui->container_main->findChildren<QWidget *>();
	foreach (QWidget *x, widgets) { x->blockSignals(b); }
}

#endif

#ifndef SEFMediaPreparer_Section_Worker_Scan

void MediaPreparer::scanLibrary(std::string dir) {
	emit started_workerScan();

	emit progress_updated_scan(QString("Checking Directory..."), 0);

	if (bf::exists(dir.c_str())) {

		ui->setting_dirOutput->setText(QString((dir + "\\Converted").c_str()));

		emit progress_updated_scan(QString("Scanning Files..."), 0);
		log("Scanning directory: " + dir, 0);

		bool recursive = ui->setting_subdirectories->isChecked();
		if (library->Init(dir, recursive)) {
			for (int i = 0; i < library->size() && cancelScan != true; i++) {
				File &file = library->getFile(i);
				scanIndex = i;

				QProcess process;
				if (bf::exists("./lib/ffprobe.exe")) {
					process.setProgram(QString("./lib/ffprobe"));
				} else if (bf::exists("../lib/ffprobe.exe")) {
					process.setProgram(QString("../lib/ffprobe"));
				} else if (bf::exists("../../lib/ffprobe.exe")) {
					process.setProgram(QString("../../lib/ffprobe"));
				} else {
					process.setProgram(QString("ffprobe"));
				}
				process.setArguments(workerScanParams(file));

				emit progress_updated_scan(QString((file.name() + file.extension()).c_str()));
				log("Reading File: " + file.path(), 0);

				process.start();
				process.waitForFinished();
				StringStream out(process.readAllStandardOutput());
				file.loadFileInfo(out);
				emit item_added_scan(i);
				emit progress_updated_scan((int)(((double)i / (double)library->size()) * 1000.0));
			}
			setEncodeOptions();
			library->duration();
			library->scanEncode();
			string temp = "Encode [" + std::to_string(library->sizeEncode()) + "]";
			if (library->sizeEncode() != NULL) {
				ui->button_encode->setText(QString(temp.c_str()));
			}
		}
	}

	if (!cancelScan) {
		log(QString(("Scan Completed: " + std::to_string(library->size()) + " file(s) found").c_str()), 0);
		if (library->size() > 0) {
			emit progress_updated_scan(QString("Complete..."), 1000);
		} else {
			emit progress_updated_scan(QString("Invalid Directory..."), 0);
		}
	} else {
		log(QString("Scan Cancelled"), 1);
		emit progress_updated_scan(QString("Canceled..."), 0);
	}

	emit finished_workerScan();
}

QStringList MediaPreparer::workerScanParams(SuperEpicFuntime::File &file) {
	QStringList params;
	params.push_back(QString("-v"));
	params.push_back(QString("quiet"));
	params.push_back(QString("-show_entries"));
	params.push_back(QString("format=duration:stream=codec_type:stream=codec_name"));
	params.push_back(QString("-of"));
	params.push_back(QString("json"));
	params.push_back(QString(file.path().c_str()));
	return params;
}

void MediaPreparer::runWorkerScan() {
	if (!workerScan.isRunning() && workerScan.isFinished()) {
		workerScan = QtConcurrent::run(this, &MediaPreparer::scanLibrary, getPath());
	}
}

void MediaPreparer::workerScanStart() {
	ui->list_Library->clearContents();
	lockUILoad(true);
	ui->progress_worker->setMaximum(1000);
}

void MediaPreparer::workerScanAddItem(int pos) {
	blockAllSignals(true);
	File item = library->getFile(pos);
	ui->label_fileCount->setText(
		QString(("<html><head/><body><p>" + std::to_string(pos + 1) + " file(s) found</p></body></html>").c_str()));

	ui->list_Library->setRowCount(pos + 1);
	// ui->list_Library->setItem(pos, 0, new QTableWidgetItem());
	// ui->list_Library->item(pos, 0)->setCheckState(Qt::Checked);
	ui->list_Library->setItem(pos, 0, new QTableWidgetItem(QString(item.name().c_str())));
	ui->list_Library->setItem(pos, 1, new QTableWidgetItem(QString(item.vcodec().c_str())));
	ui->list_Library->setItem(pos, 2, new QTableWidgetItem(QString(item.acodec().c_str())));
	ui->list_Library->setItem(pos, 3, new QTableWidgetItem(QString(item.extension().c_str())));
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

#endif

#ifndef SEFMediaPreparer_Section_Worker_Encode

void MediaPreparer::encodeLibrary() {
	emit started_workerEncode();

	for (int i = 0; i < (int)library->getFilesEncode().size() && cancelEncode != true; i++) {
		File &file = library->getFileEncode(i);
		encodeIndex = i;

		emit progress_updated_scan(QString(file.name().c_str()), i);
		emit progress_updated_encode(QString("Encoding File"), 0);
		emit item_changed_encode(i);

		QProcess process;
		if (bf::exists("./lib/ffmpeg.exe")) {
			process.setProgram(QString("./lib/ffmpeg"));
		} else if (bf::exists("../lib/ffmpeg.exe")) {
			process.setProgram(QString("../lib/ffmpeg"));
		} else if (bf::exists("../../lib/ffmpeg.exe")) {
			process.setProgram(QString("../../lib/ffmpeg"));
		} else {
			process.setProgram(QString("ffmpeg"));
		}
		process.setArguments(workerEncodeParams(file));

		process.start();
		process.waitForFinished(-1);
		if (!cancelEncode) {
			if (bf::exists(settings->tempDir + "\\" + file.name() + "." + settings->container)) {
				bf::copy_file(settings->tempDir + "\\" + file.name() + "." + settings->container,
							  settings->outputDir + "\\" + file.name() + "." + settings->container);
				if (bf::exists(settings->outputDir + "\\" + file.name() + "." + settings->container)) {
					bf::remove(settings->tempDir + "\\" + file.name() + "." + settings->container);
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

QStringList MediaPreparer::workerEncodeParams(SuperEpicFuntime::File &file) {
	QStringList params;
	if (bf::exists((settings->outputDir + "\\" + file.name() + "." + settings->container).c_str())) {
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
	params.push_back(QString(file.path().c_str()));
	if (file.subtitles() == 1 && settings->subtitles.compare("Embed") == 0) {
		params.push_back(QString("-i"));
		params.push_back(QString(file.pathSub().c_str()));
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
	params.push_back(QString(("title=" + file.name()).c_str()));
	params.push_back(QString("-metadata"));
	params.push_back(QString("comment=Processed by SuperEpicFuntime Media Preparer (Reborn)"));
	params.push_back(QString("-strict"));
	params.push_back(QString("-2"));
	params.push_back(QString((settings->tempDir + "\\" + file.name() + "." + settings->container).c_str()));
	return params;
}

void MediaPreparer::workerEncodeStart() {
	lockUIEncode(true);
	ui->progress_worker->setMaximum(library->sizeEncode());
	workerTimeStamp.start();
}

void MediaPreparer::runWorkerEncode() {
	if (!workerEncode.isRunning() && workerEncode.isFinished()) {
		workerEncode = QtConcurrent::run(this, &MediaPreparer::encodeLibrary);
	}
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
	ui->value_encode_file->setText(QString(f.name().c_str()));
	ui->value_encode_file_vCodec->setText(QString(f.vcodec().c_str()));
	ui->value_encode_file_aCodec->setText(QString(f.acodec().c_str()));
	ui->value_encode_file_container->setText(QString(f.extension().c_str()));
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

#endif

} // namespace SuperEpicFuntime
