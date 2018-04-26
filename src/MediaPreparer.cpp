#include <MediaPreparer.hpp>
#include <ui_MediaPreparer.h>

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
using namespace rapidjson;
using std::string;

using namespace std; // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparer
 */
MediaPreparer::MediaPreparer(QWidget *parent) : QWidget(parent), ui(new Ui::MediaPreparer) {
	ui->setupUi(this);
	eventHandler = new EventHandler();
	settings	 = new Settings();
	library		 = new Library(settings);
	init();
	eventHandler->newEvent(CUSTOM, "this is a message?", 0, NULL, "this is data[3]?", settings);
}

MediaPreparer::~MediaPreparer() {
	saveSettings_config();
	delete ui;
}

/** ================================================================================================
 * (Section) Initilization
 */
void MediaPreparer::init() {
	this->setWindowTitle(productName.c_str());
	initGUI();
	initSignals();
}

void MediaPreparer::initGUI() {
	loadSettings_config();
	loadSettings_preset();

	ui->list_Library->setColumnWidth(0, 155);
	ui->list_Library->setColumnWidth(1, 70);
	ui->list_Library->setColumnWidth(2, 70);
	ui->list_Library->setColumnWidth(3, 70);
	ui->list_Library->setColumnWidth(4, 70);

	updateGUI_timers();
}

void MediaPreparer::initSignals() {
	connect(ui->button_scan_directory, SIGNAL(clicked()), this, SLOT(runWorker_scan()), Qt::UniqueConnection);
	connect(ui->setting_directory, SIGNAL(returnPressed()), this, SLOT(runWorker_scan()), Qt::UniqueConnection);

	connect(ui->button_encode, SIGNAL(clicked()), this, SLOT(runWorker_encode()), Qt::UniqueConnection);

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

	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateGUI_timers()));

	connect(eventHandler, SIGNAL(eventAdded(Event *)), this, SLOT(eventListener(Event *)), Qt::UniqueConnection);
}

/** ================================================================================================
 * (Section) Load Settings
 */
void MediaPreparer::loadSettings_gui() {
	settings->vCodec	  = ba::trim_copy(ui->setting_vCodec->currentText().toStdString());
	settings->aCodec	  = ba::trim_copy(ui->setting_aCodec->currentText().toStdString());
	settings->vQuality	= ba::trim_copy(std::to_string(ui->setting_vQuality->value()));
	settings->aQuality	= ba::trim_copy(std::to_string(ui->setting_aQuality->value()));
	settings->container   = ba::trim_copy(ui->setting_container->currentText().toStdString());
	settings->subtitles   = ba::trim_copy(ui->setting_subtitles->currentText().toStdString());
	settings->libraryDir  = ba::trim_copy(ui->setting_directory->text().toStdString());
	settings->outputDir   = ba::trim_copy(ui->setting_dirOutput->text().toStdString());
	settings->threads	 = ba::trim_copy(ui->setting_threads->text().toStdString());
	settings->extraParams = ba::trim_copy(ui->setting_extraParams->text().toStdString());
}

void MediaPreparer::loadSettings_config() {
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

void MediaPreparer::loadSettings_preset(QString preset) {
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

void MediaPreparer::loadSettings_preset() {
	loadSettings_preset(settings->presetPath.c_str());
}

void MediaPreparer::loadSettings_presets() {
	ui->setting_preset->clear();
	settings->refreshPresets();
	for (int i = 0; i < (int)settings->presetNameList.size(); i++) {
		ui->setting_preset->addItem(settings->presetNameList[i].c_str());
	}
}

/** ================================================================================================
 * (Section) Save Settings
 */
void MediaPreparer::saveSettings_config() {
	loadSettings_gui();
	settings->saveConfig();
	loadSettings_config();
}

void MediaPreparer::saveSettings_preset(QString preset) {
	loadSettings_gui();
	settings->savePresetAs(preset.toStdString());
	loadSettings_preset(preset);
}

void MediaPreparer::saveSettings_preset() {
	loadSettings_gui();
	settings->savePreset();
	loadSettings_preset(settings->presetPath.c_str());
}

/** ================================================================================================
 * (Section) Update GUI
 */
void MediaPreparer::updateGUI_settings() {
}

void MediaPreparer::updateGUI_timers() {
	if (worker.isRunning() && workerType == ENCODE) {
		if (workerTimeStamp.isValid()) {
			ui->value_encode_runtime->setText(QString("%1:%2:%3")
												  .arg(workerTimeStamp.elapsed() / 3600000, 2, 10, QChar('0'))
												  .arg((workerTimeStamp.elapsed() % 3600000) / 60000, 2, 10, QChar('0'))
												  .arg(((workerTimeStamp.elapsed() % 3600000) % 60000) / 1000, 2, 10, QChar('0')));
		}
		if (workerItemTimeStamp.isValid()) {
			ui->progress_secondary->setFormat(QString("%1:%2:%3")
												  .arg(workerItemTimeStamp.elapsed() / 3600000, 2, 10, QChar('0'))
												  .arg((workerItemTimeStamp.elapsed() % 3600000) / 60000, 2, 10, QChar('0'))
												  .arg(((workerItemTimeStamp.elapsed() % 3600000) % 60000) / 1000, 2, 10, QChar('0')));
			ui->progress_secondary->setValue(workerItemTimeStamp.elapsed());
			ui->progress_secondary->repaint();
		}
	}
}

/** ================================================================================================
 * (Section) Workers
 */
void MediaPreparer::runWorker_scan() {
	if (!worker.isRunning()) {
		loadSettings_gui();
		worker = QtConcurrent::run(this, &MediaPreparer::scanLibrary);
	}
}

void MediaPreparer::runWorker_encode() {
	if (!worker.isRunning()) {
		loadSettings_gui();
		ui->container_settings_tabs->setCurrentIndex(3);
		worker = QtConcurrent::run(this, &MediaPreparer::encodeLibrary);
	} else {
		cancel();
	}
}

void MediaPreparer::runWorker_cleanup() {
}

/** ================================================================================================
 * (Section) Scan Worker
 */
void MediaPreparer::scanLibrary() {
	eventHandler->newEvent(WORKER_SCAN_STARTED, "Scanning Library");
	eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, 0);
	library->scan();
	eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, library->size());
	for (int i = 0; !cancelWorker && i < library->size(); i++) {
		File &f = library->getFile(i);
		eventHandler->newEvent(WORKER_SCAN_ITEM_STARTED, "Scanning File: " + f.name(), i);
		QList<QString> params = {"-v", "quiet", "-show_entries", "format=duration:stream=codec_type:stream=codec_name", "-of", "json", f.path().c_str()};
		QProcess process;
		bool r = false;
		for (int j = 0; !cancelWorker && !r && j < RETRY_COUNT; j++) {
			process.start("ffprobe", params);
			process.waitForFinished();
			StringStream out(process.readAllStandardOutput());
			r = f.loadFileInfo(out);
		}
		eventHandler->newEvent(WORKER_SCAN_ITEM_FINISHED, i);
	}
	library->scanEncode();
	if (cancelWorker) {
		eventHandler->newEvent(WORKER_SCAN_ERRORED, "Cancelled Scanning Library");
	} else {
		eventHandler->newEvent(WORKER_SCAN_FINISHED, "Finished Scanning Library");
	}
}

/** ================================================================================================
 * (Section) Encode Worker
 */
void MediaPreparer::encodeLibrary() {
	eventHandler->newEvent(WORKER_ENCODE_STARTED, "Encoding Library");
	eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, 0);
	library->scanEncode();
	eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, library->sizeEncode());
	for (int i = 0; !cancelWorker && i < (int)library->sizeEncode(); i++) {
		File &f = library->getFileEncode(i);
		eventHandler->newEvent(WORKER_ENCODE_ITEM_STARTED, "Encoding File: " + f.name(), i);
		QList<QString> params = {"-y", "-v", "quiet", "-stats", "-hwaccel", "dxva2", "-threads", settings->threads.c_str(), "-i", f.path().c_str()};
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
		params += {"-c:v", settings->vCodec.c_str(), "-crf", settings->vQuality.c_str(), "-c:a", settings->aCodec.c_str(), "-b:a", (settings->aQuality + "k").c_str()};
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
		if (!cancelWorker) {
			bf::rename(settings->tempDir + "\\" + f.name() + "." + settings->container, settings->outputDir + "\\" + f.name() + "." + settings->container);
		}
		eventHandler->newEvent(WORKER_ENCODE_ITEM_FINISHED, i);
	}
	if (cancelWorker) {
		eventHandler->newEvent(WORKER_ENCODE_ERRORED, "Cancelled Encoding Library");
	} else {
		eventHandler->newEvent(WORKER_ENCODE_FINISHED, "Finished Encoding Library");
	}
}

/** ================================================================================================
 * (Section) Event Listener
 */
void MediaPreparer::eventListener(Event *e) {
	blockSignals(true);
	EventType eventType = e->getType();
	string eventMessage = e->getMessage();
	switch (eventType) {
		/** ============================================================================================
		 * (Event) WORKER_SCAN_STARTED
		 */
		case WORKER_SCAN_STARTED: {
			updateTimer->start(100);
			workerType = SCAN;
			workerTimeStamp.start();
			cancelWorker = false;
			ui->progress_primary->setValue(0);
			lockUI(true);
			ui->list_Library->clearContents();
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_SCAN_FINISHED
		 */
		case WORKER_SCAN_FINISHED: {
			updateTimer->stop();
			if (!eventMessage.empty()) {
				ui->progress_primary->setFormat(eventMessage.c_str());
			}
			lockUI(false);
			ui->progress_primary->setValue(library->size());
			ui->button_encode->setText(("Encode [" + to_string(library->sizeEncode()) + "]").c_str());
			ui->button_encode->setEnabled((library->sizeEncode() > 0));
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_SCAN_ERRORED
		 */
		case WORKER_SCAN_ERRORED: {
			updateTimer->stop();
			if (!eventMessage.empty()) {
				ui->progress_primary->setFormat(eventMessage.c_str());
			}
			lockUI(false);
			ui->progress_primary->setValue(0);
			ui->button_encode->setText("Encode [0]");
			ui->button_encode->setEnabled(false);
			break;
		}
		/** ================================================================================================
		 * (Event) WORKER_SCAN_ITEM_STARTED
		 */
		case WORKER_SCAN_ITEM_STARTED: {
			if (e->dataIsType<int>()) {
				int eventData = e->getData<int>();
				workerItemTimeStamp.start();
				if (!eventMessage.empty()) {
					ui->progress_primary->setFormat(eventMessage.c_str());
				}
				workerItem = library->getFile(eventData);
				ui->label_fileCount->setText(("<html><head/><body><p>" + std::to_string(eventData + 1) + " file(s) found</p></body></html>").c_str());
			}
			break;
		}
		/** ================================================================================================
		 * (Event) WORKER_SCAN_ITEM_FINISHED
		 */
		case WORKER_SCAN_ITEM_FINISHED: {
			if (e->dataIsType<int>()) {
				int eventData = e->getData<int>();
				ui->progress_primary->setValue(eventData);
				File &eventFile = library->getFile(eventData);
				ui->list_Library->setRowCount(eventData + 1);
				ui->list_Library->setItem(eventData, 0, new QTableWidgetItem(QString(eventFile.name().c_str())));
				ui->list_Library->setItem(eventData, 1, new QTableWidgetItem(QString(eventFile.vcodec().c_str())));
				ui->list_Library->setItem(eventData, 2, new QTableWidgetItem(QString(eventFile.acodec().c_str())));
				ui->list_Library->setItem(eventData, 3, new QTableWidgetItem(QString(eventFile.extension().c_str())));
				ui->list_Library->setItem(eventData, 4, new QTableWidgetItem(QString(eventFile.subtitlesStr().c_str())));
			}
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_ENCODE_STARTED
		 */
		case WORKER_ENCODE_STARTED: {
			updateTimer->start(100);
			workerType = ENCODE;
			workerTimeStamp.start();
			cancelWorker = false;
			ui->progress_primary->setValue(0);
			ui->progress_secondary->setValue(0);
			lockUI(true);
			ui->list_encode_Library->clearContents();
			ui->value_encode_vCodec->setText(settings->vCodec.c_str());
			ui->value_encode_vQuality->setText(settings->vQuality.c_str());
			ui->value_encode_aCodec->setText(settings->aCodec.c_str());
			ui->value_encode_aQuality->setText(settings->aQuality.c_str());
			ui->value_encode_container->setText(settings->container.c_str());
			ui->value_encode_subtitles->setText(settings->subtitles.c_str());
			for (int i = 0; i < library->sizeEncode(); i++) {
				File &f = library->getFileEncode(i);
				ui->list_encode_Library->setRowCount(i + 1);
				ui->list_encode_Library->setItem(i, 0, new QTableWidgetItem(QString(f.name().c_str())));
				ui->list_encode_Library->setItem(i, 1, new QTableWidgetItem(QString(f.vcodec().c_str())));
				ui->list_encode_Library->setItem(i, 2, new QTableWidgetItem(QString(f.acodec().c_str())));
				ui->list_encode_Library->setItem(i, 3, new QTableWidgetItem(QString(f.extension().c_str())));
				ui->list_encode_Library->setItem(i, 4, new QTableWidgetItem(QString(f.subtitlesStr().c_str())));
			}
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_ENCODE_FINISHED
		 */
		case WORKER_ENCODE_FINISHED: {
			updateTimer->stop();
			if (!eventMessage.empty()) {
				ui->progress_primary->setFormat(eventMessage.c_str());
			}
			lockUI(false);
			ui->button_encode->setText(("Encode [" + to_string(library->sizeEncode()) + "]").c_str());
			ui->button_encode->setEnabled((library->sizeEncode() > 0));
			ui->progress_primary->setValue(library->sizeEncode());
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_ENCODE_ERRORED
		 */
		case WORKER_ENCODE_ERRORED: {
			updateTimer->stop();
			if (!eventMessage.empty()) {
				ui->progress_primary->setFormat(eventMessage.c_str());
			}
			lockUI(false);
			ui->button_encode->setText(("Encode [" + to_string(library->sizeEncode()) + "]").c_str());
			ui->button_encode->setEnabled((library->sizeEncode() > 0));
			ui->progress_primary->setValue(0);
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_ENCODE_ITEM_STARTED
		 */
		case WORKER_ENCODE_ITEM_STARTED: {
			if (e->dataIsType<int>()) {
				int eventData = e->getData<int>();
				workerItemTimeStamp.start();
				if (!eventMessage.empty()) {
					ui->progress_primary->setFormat(eventMessage.c_str());
				}
				workerItem = library->getFileEncode(eventData);
				ui->list_encode_Library->selectRow(eventData);
				ui->value_encode_file->setText(workerItem.name().c_str());
				ui->value_encode_file_vCodec->setText(workerItem.vcodec().c_str());
				ui->value_encode_file_aCodec->setText(workerItem.acodec().c_str());
				ui->value_encode_file_container->setText(ba::replace_all_copy(workerItem.extension(), ".", "").c_str());
				ui->value_encode_file_subtitles->setText(workerItem.subtitlesStr().c_str());
				ui->value_encode_file_duration->setText(QString("%1:%2:%3")
															.arg(workerItem.duration() / 3600000, 2, 10, QChar('0'))
															.arg((workerItem.duration() % 3600000) / 60000, 2, 10, QChar('0'))
															.arg(((workerItem.duration() % 3600000) % 60000) / 1000, 2, 10, QChar('0')));
				ui->progress_secondary->setMaximum(workerItem.duration());
				ui->value_encode_count->setText((to_string(eventData + 1) + " / " + to_string(library->sizeEncode())).c_str());
			}
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_ENCODE_ITEM_FINISHED
		 */
		case WORKER_ENCODE_ITEM_FINISHED: {
			if (e->dataIsType<int>()) {
				int eventData   = e->getData<int>();
				File &eventFile = library->getFileEncode(eventData);
				ui->value_encode_lastFile->setText(QString("%1:%2:%3")
													   .arg(workerItemTimeStamp.elapsed() / 3600000, 2, 10, QChar('0'))
													   .arg((workerItemTimeStamp.elapsed() % 3600000) / 60000, 2, 10, QChar('0'))
													   .arg(((workerItemTimeStamp.elapsed() % 3600000) % 60000) / 1000, 2, 10, QChar('0')));
			}
			break;
		}
		/** ============================================================================================
		 * (Event) PROGRESS_PRIMARY_UPDATED
		 */
		case PROGRESS_PRIMARY_UPDATED: {
			if (e->dataIsType<int>()) {
				int eventData = e->getData<int>();
				ui->progress_primary->setValue(eventData);
				if (!eventMessage.empty()) {
					ui->progress_primary->setFormat(eventMessage.c_str());
				}
			}
			break;
		}
		/** ============================================================================================
		 * (Event) PROGRESS_PRIMARY_MAXIMUM_CHANGED
		 */
		case PROGRESS_PRIMARY_MAXIMUM: {
			if (e->dataIsType<int>()) {
				int eventData = e->getData<int>();
				ui->progress_primary->setMaximum(eventData);
			}
			break;
		}
		default: {
			cout << "Unhandled Event | Type: " << e->getTypeStr() << " | Message: " << eventMessage << endl;
			break;
		}
	}
	blockSignals(false);
}

/** ================================================================================================
 * (Section) Dialogs
 */
void MediaPreparer::dialogBrowse(int type) {
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setDirectory(QDir(ui->setting_directory->text()));
	dialog.exec();
	if (dialog.result() == QDialog::Accepted) {
		QStringList dir = dialog.selectedFiles();
		switch (type) {
			case 0: {
				ui->setting_directory->setText(dir[0]);
				runWorker_scan();
				break;
			}
			case 1: {
				ui->setting_dirOutput->setText(dir[0]);
				break;
			}
		}
	}
}

void MediaPreparer::dialogSave() {
	QString fileName =
		QFileDialog::getSaveFileName(this, "Save Preset - Will only be loaded from preset directory", (settings->baseDir + "//presets//Custom").c_str(), "SEF Preset (*.preset)");
	if (!fileName.isEmpty()) {
		saveSettings_preset(fileName);
	}
}

bool MediaPreparer::dialogCancel() {
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

/** ================================================================================================
 * (Section) Utilities
 */
bool MediaPreparer::cancel() {
	if (!dialogCancel()) {
		return false;
	}
	if (worker.isRunning()) {
		QProcess kill;
		cancelWorker = true;
		switch (workerType) {
			case SCAN:
				kill.start(QString("taskkill /F /T /IM ffprobe.exe"));
				kill.waitForFinished();
				worker.waitForFinished();
				break;
			case ENCODE:
				kill.start(QString("taskkill /F /T /IM ffmpeg.exe"));
				kill.waitForFinished();
				worker.waitForFinished();
				if (bf::exists(settings->tempDir + "\\" + workerItem.name() + "." + settings->container)) {
					bf::remove(settings->tempDir + "\\" + workerItem.name() + "." + settings->container);
				}
				break;
			default:

				break;
		}
	}
	return cancelWorker;
}

void MediaPreparer::lockUI(bool b) {
	ui->setting_directory->setEnabled(!b);
	ui->button_browse_directory->setEnabled(!b);
	ui->button_scan_directory->setEnabled(!b);
	ui->setting_dirOutput->setEnabled(!b);
	ui->button_browse_dirOutput->setEnabled(!b);
	ui->setting_vCodec->setEnabled(!b);
	ui->setting_vQuality->setEnabled(!b);
	ui->setting_aCodec->setEnabled(!b);
	ui->setting_aQuality->setEnabled(!b);
	ui->setting_container->setEnabled(!b);
	ui->setting_subtitles->setEnabled(!b);
	ui->setting_subdirectories->setEnabled(!b);
	ui->setting_threads->setEnabled(!b);
	ui->setting_extraParams->setEnabled(!b);
	ui->setting_preset->setEnabled(!b);
	if (b) {
		ui->button_encode->setText("Cancel");
		ui->button_encode->setEnabled(true);
	}
}

void MediaPreparer::blockSignals(bool b) {
	QList<QWidget *> widgets = ui->container_main->findChildren<QWidget *>();
	foreach (QWidget *x, widgets) { x->blockSignals(b); }
}

void MediaPreparer::closeEvent(QCloseEvent *e) {
	if (!worker.isRunning() || cancel()) {
		e->accept();
	} else {
		e->ignore();
	}
}

} // namespace SuperEpicFuntime
