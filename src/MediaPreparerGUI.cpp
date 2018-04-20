#include "src/MediaPreparerGUI.hpp"
#include "ui_MediaPreparer.h"

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
using namespace rapidjson;
using namespace std;

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparer
 */
MediaPreparerGUI::MediaPreparerGUI(QWidget *parent) : QWidget(parent), ui(new Ui::MediaPreparer) {
	ui->setupUi(this);
	eventHandler = new EventHandler();
	settings = new Settings();
	library = new Library(settings);
	init();
}

MediaPreparerGUI::~MediaPreparerGUI() {
	cancelWorker = true;
	saveSettings_config();
}

/** ================================================================================================
 * (Section) Initilization
 */
void MediaPreparerGUI::init() {
	initGUI();
	initSignals();
}

void MediaPreparerGUI::initGUI() {
	loadSettings_config();
	loadSettings_preset(settings->presetPath.c_str());
}

void MediaPreparerGUI::initSignals() {
	connect(this, SIGNAL(signal_loadSettings_gui()), this, SLOT(loadSettings_gui()));
	connect(this, SIGNAL(signal_loadSettings_config()), this, SLOT(loadSettings_config()));
	connect(this, SIGNAL(signal_loadSettings_preset(QString)), this, SLOT(loadSettings_preset(QString)));
	connect(this, SIGNAL(signal_saveSettings_config()), this, SLOT(saveSettings_config()));
	connect(this, SIGNAL(signal_saveSettings_preset(QString)), this, SLOT(saveSettings_preset(QString)));
	connect(this, SIGNAL(signal_updateGUI_settings()), this, SLOT(updateGUI_settings()));
	connect(this, SIGNAL(signal_updateGUI_timers()), this, SLOT(updateGUI_timers()));
	connect(this, SIGNAL(signal_runWorker_scan()), this, SLOT(runWorker_scan()));
	connect(this, SIGNAL(signal_runWorker_encode()), this, SLOT(runWorker_encode()));
	connect(this, SIGNAL(signal_runWorker_cleanup()), this, SLOT(runWorker_cleanup()));
	connect(this, SIGNAL(signal_dialogSave()), this, SLOT(dialogSave()));
	connect(this, SIGNAL(signal_dialogCancel()), this, SLOT(dialogCancel()));
	connect(this, SIGNAL(signal_log(QString)), this, SLOT(log(QString)));
	connect(this, SIGNAL(signal_blockSignals(bool)), this, SLOT(blockSignals(bool)));

	connect(ui->button_scan_directory, SIGNAL(clicked()), this, SLOT(runWorker_scan()));
	connect(ui->setting_directory, SIGNAL(returnPressed()), this, SLOT(runWorker_scan()));

	connect(ui->button_savePreset, SIGNAL(clicked()), this, SLOT(dialogSave()));

	connect(ui->button_browse_directory, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(ui->button_browse_directory, 0);
	connect(ui->button_browse_dirOutput, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(ui->button_browse_dirOutput, 1);
	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(dialogBrowse(int)));

	connect(ui->setting_vCodec, SIGNAL(currentIndexChanged(int)), this, SLOT(loadSettings_gui()));
	connect(ui->setting_aCodec, SIGNAL(currentIndexChanged(int)), this, SLOT(loadSettings_gui()));
	connect(ui->setting_container, SIGNAL(currentIndexChanged(int)), this, SLOT(loadSettings_gui()));
	connect(ui->setting_preset, SIGNAL(currentTextChanged(QString)), this, SLOT(loadSettings_preset(QString)));

	connect(eventHandler, SIGNAL(addedEvent(int)), this, SLOT(eventListener(int)));
}

/** ================================================================================================
 * (Section) Load Settings
 */
void MediaPreparerGUI::loadSettings_gui() {
	settings->vCodec = ba::trim_copy(ui->setting_vCodec->currentText().toStdString());
	settings->aCodec = ba::trim_copy(ui->setting_aCodec->currentText().toStdString());
	settings->vQuality = ba::trim_copy(std::to_string(ui->setting_vQuality->value()));
	settings->aQuality = ba::trim_copy(std::to_string(ui->setting_aQuality->value()));
	settings->container = ba::trim_copy(ui->setting_container->currentText().toStdString());
	settings->subtitles = ba::trim_copy(ui->setting_subtitles->currentText().toStdString());
	settings->libraryDir = ba::trim_copy(ui->setting_directory->text().toStdString());
	settings->outputDir = ba::trim_copy(ui->setting_dirOutput->text().toStdString());
	settings->threads = ba::trim_copy(ui->setting_threads->text().toStdString());
	settings->extraParams = ba::trim_copy(ui->setting_extraParams->text().toStdString());
}

void MediaPreparerGUI::loadSettings_config() {
	blockSignals(true);
	ui->setting_vCodec->clear();
	ui->setting_aCodec->clear();
	ui->setting_container->clear();

	ui->setting_directory->setText(settings->libraryDir.c_str());
	ui->setting_dirOutput->setText(settings->outputDir.c_str());

	for (int i = 0; i < (int)settings->vCodecList.size(); i++) {
		ui->setting_vCodec->addItem(settings->vCodecList[i][0].c_str());
	}
	for (int i = 0; i < (int)settings->aCodecList.size(); i++) {
		ui->setting_aCodec->addItem(settings->aCodecList[i][0].c_str());
	}
	for (int i = 0; i < (int)settings->containerList.size(); i++) {
		ui->setting_container->addItem(settings->containerList[i].c_str());
	}
	blockSignals(false);
}

void MediaPreparerGUI::loadSettings_preset(QString preset) {
	blockSignals(true);
	loadSettings_presets();
	settings->loadPreset(preset.toStdString());
	ui->setting_preset->setCurrentText(settings->presetName.c_str());
	ui->setting_preset->setToolTip(settings->presetPath.c_str());
	ui->setting_vCodec->setCurrentText(settings->vCodec.c_str());
	ui->setting_aCodec->setCurrentText(settings->aCodec.c_str());
	ui->setting_vQuality->setValue(stoi(settings->vQuality));
	ui->setting_aQuality->setValue(stoi(settings->aQuality));
	ui->setting_container->setCurrentText(settings->container.c_str());
	ui->setting_subtitles->setCurrentText(settings->subtitles.c_str());
	ui->setting_threads->setText(settings->threads.c_str());
	ui->setting_extraParams->setText(settings->extraParams.c_str());
	blockSignals(false);
}

void MediaPreparerGUI::loadSettings_presets() {
	ui->setting_preset->clear();
	settings->refreshPresets();
	for (int i = 0; i < (int)settings->presetNameList.size(); i++) {
		ui->setting_preset->addItem(settings->presetNameList[i].c_str());
	}
}

/** ================================================================================================
 * (Section) Save Settings
 */
void MediaPreparerGUI::saveSettings_config() {
	loadSettings_gui();
	settings->saveConfig();
}

void MediaPreparerGUI::saveSettings_preset(QString preset) {
	loadSettings_gui();
	settings->savePresetAs(preset.toStdString());
	loadSettings_preset(preset);
}

/** ================================================================================================
 * (Section) Update GUI
 */
void MediaPreparerGUI::updateGUI_settings() {
}

void MediaPreparerGUI::updateGUI_timers() {
}

/** ================================================================================================
 * (Section) Workers
 */
void MediaPreparerGUI::runWorker_scan() {
	worker = QtConcurrent::run(this, &MediaPreparerGUI::scanLibrary);
}

void MediaPreparerGUI::runWorker_encode() {
	encodeLibrary();
}

void MediaPreparerGUI::runWorker_cleanup() {
}

/** ================================================================================================
 * (Section) Scan Worker
 */
void MediaPreparerGUI::scanLibrary() {
	eventHandler->newEvent(WORKER_STARTED, "Scanning Library", SCAN);
	library->scan();
	eventHandler->newEvent(PROGRESS_MAXIMUM, "Library Size: ", library->size());
	for (int i = 0; !cancelWorker && i < library->size(); i++) {
		File &f = library->getFile(i);
		eventHandler->newEvent(WORKER_ITEM_CHANGED, "SCAN", i);
		eventHandler->newEvent(PROGRESS_UPDATED, "Scanning File: " + f.name(), i);
		QList<QString> params = {"-v",  "quiet", "-show_entries", "format=duration:stream=codec_type:stream=codec_name",
								 "-of", "json",  f.path().c_str()};
		QProcess process;
		bool r = false;
		for (int j = 0; !cancelWorker && !r && j < RETRY_COUNT; j++) {
			process.start("ffprobe", params);
			process.waitForFinished();
			StringStream out(process.readAllStandardOutput());
			r = f.loadFileInfo(out);
		}
	}
	library->scanEncode();
	eventHandler->newEvent(WORKER_FINISHED, "Finished Scanning Library", SCAN);
}

/** ================================================================================================
 * (Section) Encode Worker
 */
void MediaPreparerGUI::encodeLibrary() {
	eventHandler->newEvent(WORKER_STARTED, "Encoding Library", ENCODE);
	library->scanEncode();
	for (int i = 0; !cancelWorker && i < (int)library->sizeEncode(); i++) {
		File &f = library->getFileEncode(i);
		eventHandler->newEvent(WORKER_ITEM_CHANGED, "ENCODE", i);
		eventHandler->newEvent(PROGRESS_UPDATED, "Encoding File: " + f.name(), i);
		QList<QString> params = {"-y",		 "-v",
								 "quiet",	"-stats",
								 "-hwaccel", "dxva2",
								 "-threads", settings->threads.c_str(),
								 "-i",		 f.path().c_str()};
		if (f.subtitles() == 1 && settings->subtitles.compare("Embed") == 0) {
			params += {"-i", f.pathSub().c_str()};
		}
		params += {"-map", "0:0", "-map", "0:1?"};
		if (f.subtitles() == 1 && settings->subtitles.compare("Embed") == 0) {
			params += {"-map", "1:0"};
		}
		if (f.subtitles() > 0 && settings->subtitles.compare("Remove") != 0) {
			params += {"-map", "0:2?", "-c:s", "srt", "-metadata:s:s:0", "language=eng", "-disposition:s:0", "default"};
		}
		params += {"-c:v", settings->vCodec.c_str(), "-crf", settings->vQuality.c_str(),
				   "-c:a", settings->aCodec.c_str(), "-b:a", (settings->aQuality + "k").c_str()};
		if (!settings->extraParams.empty()) {
			char s[2048];
			strcpy(s, settings->extraParams.c_str());
			for (char *p = strtok(s, " "); p != NULL; p = strtok(NULL, " ")) {
				params.push_back(p);
			}
		}
		params += {"-metadata",
				   ("title=" + f.name()).c_str(),
				   "-metadata",
				   "comment=Processed by SuperEpicFuntime Media Preparer",
				   "-strict",
				   "-2",
				   (settings->tempDir + "\\" + f.name() + "." + settings->container).c_str()};
		QProcess process;
		process.start("ffmpeg", params);
		process.waitForFinished(-1);
	}
	eventHandler->newEvent(WORKER_FINISHED, "Finished Encoding Library", ENCODE);
}

/** ================================================================================================
 * (Section) Event Listener
 */
void MediaPreparerGUI::eventListener(int pos) {
	Event p = eventHandler->getLastEvent();
	Event e = eventHandler->getEvent();
	EventType t = e.getType();
	QTime ts = e.getTimeStamp();
	string m = e.getMessage();
	int d = e.getData();
	File f;

	// if (!eventHandler->compare(p, e)) {
	cout << "[" << t << " | " << eventHandler->size() << "] " << m << " : " << d << endl;
	switch (t) {
	case PROGRESS_UPDATED:
		ui->progress_primary->setValue(d);
		if (!m.empty()) {
			ui->progress_primary->setFormat(m.c_str());
		}
		break;

	case PROGRESS_MAXIMUM:
		ui->progress_primary->setMaximum(d);
		break;

	case WORKER_STARTED:
		switch ((WorkerType)d) {
		case SCAN:

			break;
		case ENCODE:

			break;
		case CLOSE:

			break;
		}
		break;

	case WORKER_FINISHED:
		switch ((WorkerType)d) {
		case SCAN:

			break;
		case ENCODE:

			break;
		case CLOSE:

			break;
		}
		break;

	case WORKER_ITEM_CHANGED:
		if (m.compare("SCAN") == 0) {
			f = library->getFile(d);
		} else if (m.compare("ENCODE") == 0) {
			f = library->getFileEncode(d);
		}
		break;

	default:

		break;
	}
	//}
}

/** ================================================================================================
 * (Section) Dialogs
 */
void MediaPreparerGUI::dialogBrowse(int type) {
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setDirectory(QDir(ui->setting_directory->text()));
	dialog.exec();
	if (dialog.result() == QDialog::Accepted) {
		QStringList dir = dialog.selectedFiles();
		switch (type) {
		case 0:
			ui->setting_directory->setText(dir[0]);
			runWorker_scan();
			break;
		case 1:
			ui->setting_dirOutput->setText(dir[0]);
			break;
		}
	}
}

void MediaPreparerGUI::dialogSave() {
	QString fileName =
		QFileDialog::getSaveFileName(this, "Save Preset - Will only be loaded from preset directory",
									 (settings->baseDir + "//presets//Custom").c_str(), "SEF Preset (*.preset)");
	if (!fileName.isEmpty()) {
		saveSettings_preset(fileName);
	}
}

void MediaPreparerGUI::dialogCancel() {
}

/** ================================================================================================
 * (Section) Utilities
 */
void MediaPreparerGUI::log(QString msg) {
}

void MediaPreparerGUI::blockSignals(bool b) {
	QList<QWidget *> widgets = ui->container_main->findChildren<QWidget *>();
	foreach (QWidget *x, widgets) { x->blockSignals(b); }
}

void MediaPreparerGUI::closeEvent(QCloseEvent *e) {
}

} // namespace SuperEpicFuntime