#include <MediaPreparer.hpp>
#include <ui_MediaPreparer.h>

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;

namespace SuperEpicFuntime::MediaPreparer {

MediaPreparer::MediaPreparer(QWidget *parent) : QMainWindow(parent), ui(new Ui::MediaPreparer) {
	ui->setupUi(this);

	eventHandler = new EventHandler();
	connect(eventHandler, SIGNAL(eventAdded(Event *)), this, SLOT(eventListener(Event *)), Qt::UniqueConnection);
	settings = new Settings(eventHandler);
	library  = new Library(settings, eventHandler);

	init();
	eventHandler->newEvent(INITIALIZED, 0);
}

MediaPreparer::MediaPreparer(EventHandler *evn, Settings *set, Library *lib, QWidget *parent) : QMainWindow(parent), ui(new Ui::MediaPreparer) {
	ui->setupUi(this);

	eventHandler = evn;
	connect(eventHandler, SIGNAL(eventAdded(Event *)), this, SLOT(eventListener(Event *)), Qt::UniqueConnection);
	if (set == NULL) {
		settings = new Settings(eventHandler);
	} else {
		settings = set;
	}
	if (lib == NULL) {
		library = new Library(settings, eventHandler);
	} else {
		library = lib;
	}

	init();
	eventHandler->newEvent(INITIALIZED, 0);
}

MediaPreparer::~MediaPreparer() {
	saveSettings_config();
	delete ui;
	cancel(true);
	eventHandler->newEvent(TERMINATED, 1);
}

void MediaPreparer::init() {
	this->setWindowTitle(productName.c_str());
	if (!settings->preserveLog) {
		bf::remove(settings->baseDir + "\\log.txt");
	}
	initGUI();
	initSignals();
}

void MediaPreparer::initGUI() {
	loadSettings_config();
	loadSettings_preset();

	ui->list_Library->setColumnWidth(0, 200);
	ui->list_Library->setColumnWidth(1, 55);
	ui->list_Library->setColumnWidth(2, 55);
	ui->list_Library->setColumnWidth(3, 65);
	ui->list_Library->setColumnWidth(4, 70);

	ui->container_settings_tabs->setTabVisible(3, false);
	ui->container_settings_tabs->setTabVisible(4, false);
	ui->container_settings_tabs->setTabVisible(5, false);

	updateGUI_timers();
}

void MediaPreparer::initSignals() {
	connect(ui->button_scan_directory, SIGNAL(clicked()), this, SLOT(runWorker_scan()), Qt::UniqueConnection);
	connect(ui->setting_directory, SIGNAL(returnPressed()), this, SLOT(runWorker_scan()), Qt::UniqueConnection);

	connect(ui->button_encode, SIGNAL(clicked()), this, SLOT(runWorker_encode()), Qt::UniqueConnection);

	connect(ui->button_savePreset, SIGNAL(clicked()), this, SLOT(dialogSave()), Qt::UniqueConnection);

	connect(ui->button_browse_directory, SIGNAL(clicked()), signalMapper, SLOT(map()), Qt::UniqueConnection);
	signalMapper->setMapping(ui->button_browse_directory, 0);
	connect(ui->button_browse_dirOutput, SIGNAL(clicked()), signalMapper, SLOT(map()), Qt::UniqueConnection);
	signalMapper->setMapping(ui->button_browse_dirOutput, 1);
	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(dialogBrowse(int)), Qt::UniqueConnection);

	connect(ui->setting_vCodec, SIGNAL(currentIndexChanged(int)), this, SLOT(loadSettings_gui()), Qt::UniqueConnection);
	connect(ui->setting_aCodec, SIGNAL(currentIndexChanged(int)), this, SLOT(loadSettings_gui()), Qt::UniqueConnection);
	connect(ui->setting_container, SIGNAL(currentIndexChanged(int)), this, SLOT(loadSettings_gui()), Qt::UniqueConnection);
	connect(ui->setting_container, SIGNAL(currentIndexChanged(int)), this, SLOT(updateGUI_settings_container()), Qt::UniqueConnection);
	connect(ui->setting_preset, SIGNAL(currentTextChanged(QString)), this, SLOT(loadSettings_preset(QString)), Qt::UniqueConnection);
	connect(ui->setting_override, SIGNAL(stateChanged(int)), this, SLOT(overrideUI(int)), Qt::UniqueConnection);

	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateGUI_timers()));
}

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
	settings->forceEncode = ui->setting_forceEncode->isChecked();
	settings->fixMetadata = ui->setting_fixMetadata->isChecked();
	settings->subfolders = ui->setting_subfolders->isChecked();
	settings->override = ui->setting_override->isChecked();
	settings->parseOverrideParams(ba::trim_copy(ui->setting_override_args->toPlainText().toStdString()));
	settings->saveConfig();
}

void MediaPreparer::loadSettings_config() {
	blockSignals(true);
	ui->setting_vCodec->clear();
	ui->setting_aCodec->clear();
	ui->setting_container->clear();

	ui->setting_directory->setText(settings->libraryDir.c_str());
	ui->setting_dirOutput->setText(settings->outputDir.c_str());

	for (int i {0}; i < (int)settings->vCodecList.size(); i++) {
		ui->setting_vCodec->addItem(settings->vCodecList[i][0].c_str());
	}
	for (int i {0}; i < (int)settings->aCodecList.size(); i++) {
		ui->setting_aCodec->addItem(settings->aCodecList[i][0].c_str());
	}
	for (int i {0}; i < (int)settings->containerList.size(); i++) {
		ui->setting_container->addItem(settings->containerList[i].c_str());
	}
	blockSignals(false);
}

void MediaPreparer::loadSettings_preset(QString preset) {
	blockSignals(true);
	loadSettings_presets();
	settings->loadPreset(preset.toStdString());
	ui->setting_preset->blockSignals(true);
	ui->setting_preset->setCurrentText(settings->presetName.c_str());
	ui->setting_preset->setToolTip(settings->presetPath.c_str());
	ui->setting_preset->blockSignals(false);
	ui->setting_vCodec->setCurrentText(settings->vCodec.c_str());
	ui->setting_aCodec->setCurrentText(settings->aCodec.c_str());
	ui->setting_vQuality->setValue(stoi(settings->vQuality));
	ui->setting_aQuality->setValue(stoi(settings->aQuality));
	ui->setting_container->setCurrentText(settings->container.c_str());
	ui->setting_subtitles->setCurrentText(settings->subtitles.c_str());
	ui->setting_threads->setText(settings->threads.c_str());
	ui->setting_extraParams->setText(settings->extraParams.c_str());
	ui->setting_override->setChecked(settings->override);
	std::string overrideParamsString {""};
	for (int i = 0; i < settings->overrideParams.size(); ++i) {
		overrideParamsString += settings->overrideParams.at(i).toStdString() + " ";
	}
	ui->setting_override_args->setPlainText(overrideParamsString.c_str());
	blockSignals(false);
}

void MediaPreparer::loadSettings_preset() {
	loadSettings_preset(settings->presetPath.c_str());
}

void MediaPreparer::loadSettings_presets() {
	ui->setting_preset->clear();
	settings->refreshPresets();
	for (int i {0}; i < (int)settings->presetNameList.size(); i++) {
		ui->setting_preset->addItem(settings->presetNameList[i].c_str());
	}
}

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

void MediaPreparer::updateGUI_settings_container() {
	bool enableSub {false};
	for (unsigned int i {0}; i < settings->SUBTITLE_CONTAINERS.size(); i++) {
		if (ui->setting_container->currentText().toStdString() == settings->SUBTITLE_CONTAINERS[i]) enableSub = true;
	}
	if (!enableSub) {
		ui->setting_subtitles->setCurrentIndex(2);
		ui->setting_subtitles->setDisabled(true);
	} else {
		ui->setting_subtitles->setCurrentIndex(0);
		ui->setting_subtitles->setDisabled(false);
	}
}

#define END_SETTINGS_SECTION }
#define WORKER_SECTION {

void MediaPreparer::updateGUI_timers() {
	if (workerThread.isRunning() && workerType == ENCODE) {
		if (workerTimeStamp.isValid()) {
			ui->value_encode_runtime->setText(QString("%1:%2:%3")
												  .arg(workerTimeStamp.elapsed() / 3600000, 2, 10, QChar('0'))
												  .arg((workerTimeStamp.elapsed() % 3600000) / 60000, 2, 10, QChar('0'))
												  .arg(((workerTimeStamp.elapsed() % 3600000) % 60000) / 1000, 2, 10, QChar('0')));
		}
	}
}

void MediaPreparer::runWorker_scan() {
	if (!workerThread.isRunning()) {
		loadSettings_gui();
		worker		 = Worker(SCAN);
		workerThread = QtConcurrent::run(worker, &Worker::run);
	}
}

void MediaPreparer::runWorker_encode() {
	if (!workerThread.isRunning()) {
		loadSettings_gui();
		worker		 = Worker(ENCODE);
		workerThread = QtConcurrent::run(worker, &Worker::run);
	} else {
		cancel();
	}
}

void MediaPreparer::runWorker_cleanup() {
}

void MediaPreparer::eventListener(Event *e) {
	blockSignals(true);
	EventType eventType {e->getType()};
	std::string eventMessage {e->getMessage()};
	switch (eventType) {
	case WORKER_SCAN_STARTED: {
		updateTimer->start(100);
		workerType = SCAN;
		workerTimeStamp.start();
		cancelWorker = false;
		ui->progress_primary->setValue(0);
		lockUI(true);
		blockSignals(false);
		if (ui->container_settings_tabs->currentIndex() >= 3) {
			ui->container_settings_tabs->setCurrentIndex(0);
			ui->container_settings_tabs->setCurrentWidget(ui->container_settings_basic);
		}
		ui->container_settings_tabs->setTabVisible(3, false);
		ui->container_settings_tabs->setTabVisible(4, false);
		ui->container_settings_tabs->setTabVisible(5, false);
		ui->list_Library->clearContents();
		blockSignals(true);
		break;
	}
	case WORKER_SCAN_FINISHED: {
		updateTimer->stop();
		if (!eventMessage.empty()) {
			ui->progress_primary->setFormat(eventMessage.c_str());
		}
		lockUI(false);
		ui->progress_primary->setValue(library->size());
		ui->button_encode->setText(("Encode [" + std::to_string(library->sizeEncode()) + "]").c_str());
		ui->button_encode->setEnabled((library->sizeEncode() > 0));
		break;
	}
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
	case WORKER_SCAN_ITEM_STARTED: {
		if (e->dataIsType<int>(0)) {
			int eventData {e->getData<int>(0)};
			workerItemTimeStamp.start();
			if (!eventMessage.empty()) {
				ui->progress_primary->setFormat(eventMessage.c_str());
			}
			workerItem = library->getFile(eventData);
			ui->label_fileCount->setText(("<html><head/><body><p>" + std::to_string(eventData + 1) + " file(s) found</p></body></html>").c_str());
		}
		break;
	}
	case WORKER_SCAN_ITEM_FINISHED: {
		if (e->dataIsType<int>(0)) {
			int eventData {e->getData<int>(0)};
			ui->progress_primary->setValue(eventData);
			File &eventFile {library->getFile(eventData)};
			ui->list_Library->setRowCount(eventData + 1);
			ui->list_Library->setItem(eventData, 0, new QTableWidgetItem(QString(eventFile.name().c_str())));
			ui->list_Library->setItem(eventData, 1, new QTableWidgetItem(QString(eventFile.vcodec().c_str())));
			ui->list_Library->setItem(eventData, 2, new QTableWidgetItem(QString(eventFile.acodec().c_str())));
			ui->list_Library->setItem(eventData, 3, new QTableWidgetItem(QString(eventFile.extension().c_str())));
			ui->list_Library->setItem(eventData, 4, new QTableWidgetItem(QString(eventFile.subtitlesStr().c_str())));
		}
		break;
	}
	case WORKER_ENCODE_STARTED: {
		updateTimer->start(100);
		workerType = ENCODE;
		workerTimeStamp.start();
		cancelWorker = false;
		ui->progress_primary->setValue(0);
		ui->progress_secondary->setValue(0);
		lockUI(true);
		ui->list_encode_Library->clearContents();
		ui->list_complete_Library->clearContents();
		ui->value_encode_vCodec->setText(settings->vCodec.c_str());
		ui->value_encode_vQuality->setText(settings->vQuality.c_str());
		ui->value_encode_aCodec->setText(settings->aCodec.c_str());
		ui->value_encode_aQuality->setText(settings->aQuality.c_str());
		ui->value_encode_container->setText(settings->container.c_str());
		ui->value_encode_subtitles->setText(settings->subtitles.c_str());
		blockSignals(false);
		ui->container_settings_tabs->setTabVisible(3, true);
		ui->container_settings_tabs->setTabVisible(4, true);
		ui->container_settings_tabs->setTabVisible(5, true);
		ui->container_settings_tabs->setCurrentIndex(3);
		ui->container_settings_tabs->setCurrentWidget(ui->container_encode);
		blockSignals(true);
		for (int i {0}; i < library->sizeEncode(); i++) {
			File &f {library->getFileEncode(i)};
			ui->list_encode_Library->setRowCount(i + 1);
			ui->list_encode_Library->setItem(i, 0, new QTableWidgetItem(QString(f.name().c_str())));
			ui->list_encode_Library->setItem(i, 1, new QTableWidgetItem(QString(f.vcodec().c_str())));
			ui->list_encode_Library->setItem(i, 2, new QTableWidgetItem(QString(f.acodec().c_str())));
			ui->list_encode_Library->setItem(i, 3, new QTableWidgetItem(QString(f.extension().c_str())));
			ui->list_encode_Library->setItem(i, 4, new QTableWidgetItem(QString(f.subtitlesStr().c_str())));
		}
		break;
	}
	case WORKER_ENCODE_FINISHED: {
		updateTimer->stop();
		if (!eventMessage.empty()) {
			ui->progress_primary->setFormat(eventMessage.c_str());
		}
		lockUI(false);
		ui->button_encode->setText(("Encode [" + std::to_string(library->sizeEncode()) + "]").c_str());
		ui->button_encode->setEnabled((library->sizeEncode() > 0));
		ui->progress_primary->setValue(library->sizeEncode());
		break;
	}
	case WORKER_ENCODE_ERRORED: {
		updateTimer->stop();
		if (!eventMessage.empty()) {
			ui->progress_primary->setFormat(eventMessage.c_str());
		}
		lockUI(false);
		ui->button_encode->setText(("Encode [" + std::to_string(library->sizeEncode()) + "]").c_str());
		ui->button_encode->setEnabled((library->sizeEncode() > 0));
		ui->progress_primary->setValue(0);
		break;
	}
	case WORKER_ENCODE_ITEM_STARTED: {
		if (e->dataIsType<int>(0)) {
			int eventData {e->getData<int>(0)};
			workerItemTimeStamp.start();
			if (!eventMessage.empty()) {
				ui->progress_primary->setFormat(eventMessage.c_str());
			}
			workerItem = library->getFileEncode(eventData);
			ui->value_encode_file->setText(workerItem.name().c_str());
			ui->value_encode_file_vCodec->setText(workerItem.vcodec().c_str());
			ui->value_encode_file_aCodec->setText(workerItem.acodec().c_str());
			ui->value_encode_file_container->setText(ba::replace_all_copy(workerItem.extension(), ".", "").c_str());
			ui->value_encode_file_subtitles->setText(workerItem.subtitlesStr().c_str());
			ui->value_encode_file_duration->setText(QString("%1:%2:%3")
														.arg(workerItem.duration() / 3600000, 2, 10, QChar('0'))
														.arg((workerItem.duration() % 3600000) / 60000, 2, 10, QChar('0'))
														.arg(((workerItem.duration() % 3600000) % 60000) / 1000, 2, 10, QChar('0')));
			ui->progress_secondary->setMaximum(0);
			ui->progress_secondary->setValue(0);
			ui->progress_secondary->repaint();
			ui->value_encode_count->setText((std::to_string(eventData + 1) + " / " + std::to_string(library->sizeEncode())).c_str());
		}
		break;
	}
	case WORKER_ENCODE_ITEM_FINISHED: {
		if (e->dataIsType<int>(0)) {
			int eventData {e->getData<int>(0)};
			File &eventFile {library->getFileEncode(eventData)};
			ui->progress_secondary->setMaximum(1);
			ui->progress_secondary->setValue(1);
			ui->progress_secondary->repaint();
			ui->value_encode_lastFile->setText(QString("%1:%2:%3")
												   .arg(workerItemTimeStamp.elapsed() / 3600000, 2, 10, QChar('0'))
												   .arg((workerItemTimeStamp.elapsed() % 3600000) / 60000, 2, 10, QChar('0'))
												   .arg(((workerItemTimeStamp.elapsed() % 3600000) % 60000) / 1000, 2, 10, QChar('0')));
			ui->list_encode_Library->removeRow(0);
			ui->list_complete_Library->setRowCount(eventData + 1);
			ui->list_complete_Library->setItem(eventData, 0, new QTableWidgetItem(QString(eventFile.name().c_str())));
			ui->list_complete_Library->setItem(eventData, 1, new QTableWidgetItem(QString("%1:%2:%3")
																				  .arg(workerItemTimeStamp.elapsed() / 3600000, 2, 10, QChar('0'))
																				  .arg((workerItemTimeStamp.elapsed() % 3600000) / 60000, 2, 10, QChar('0'))
																				  .arg(((workerItemTimeStamp.elapsed() % 3600000) % 60000) / 1000, 2, 10, QChar('0'))));
		}
		break;
	}
	case PROGRESS_PRIMARY_UPDATED: {
		if (e->dataIsType<int>(0)) {
			int eventData {e->getData<int>(0)};
			ui->progress_primary->setValue(eventData);
			if (!eventMessage.empty()) {
				ui->progress_primary->setFormat(eventMessage.c_str());
			}
		}
		break;
	}
	case PROGRESS_PRIMARY_MAXIMUM: {
			if (e->dataIsType<int>(0)) {
				int eventData {e->getData<int>(0)};
				ui->progress_primary->setMaximum(eventData);
			}
			break;
	}
	case PROGRESS_SECONDARY_UPDATED: {
		if (e->dataIsType<int>(0)) {
			int eventData {e->getData<int>(0)};
			ui->progress_secondary->setValue(eventData);
			if (!eventMessage.empty()) {
				ui->progress_secondary->setFormat(eventMessage.c_str());
			}
		}
		break;
	}
	case PROGRESS_SECONDARY_MAXIMUM: {

		break;
	}
	case CONFIG_SAVED: {

		break;
	}
	case CONFIG_LOADED: {

		break;
	}
	case PRESET_SAVED: {

		break;
	}
	case PRESET_LOADED: {

		break;
	}
	default: { break; }
	}
	blockSignals(false);
}

void MediaPreparer::dialogBrowse(int type) {
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly, true);
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setDirectory(QDir(ui->setting_directory->text()));
	dialog.exec();
	if (dialog.result() == QDialog::Accepted) {
		QStringList dir {dialog.selectedFiles()};
		switch (type) {
			case 0: {
				ui->setting_directory->setText(dir[0]);
				ui->setting_dirOutput->setText(dir[0] + "/Converted");
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
	QString fileName {QFileDialog::getSaveFileName(this, "Save Preset - Will only be loaded from preset directory", (settings->baseDir + "//presets//Custom").c_str(), "SEF Preset (*.preset)")};
	if (!fileName.isEmpty()) {
		saveSettings_preset(fileName);
	}
}

bool MediaPreparer::dialogCancel() {
	QMessageBox confirm {};
	confirm.setText("Are you sure?");
	confirm.setInformativeText("Any unfinished progress will be lost.");
	confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	confirm.setDefaultButton(QMessageBox::No);
	int opt {confirm.exec()};
	if (opt == QMessageBox::No) {
		return false;
	}
	return true;
}

bool MediaPreparer::cancel(bool force) {
	if (!force && !dialogCancel()) {
		return false;
	}
	if (workerThread.isRunning()) {
		QProcess kill{};
		cancelWorker = true;
		switch (workerType) {
			case SCAN:
				if (force) {
					kill.startDetached("taskkill", {"/F", "/T", "/IM", "ffprobe.exe"});
				} else {
					kill.start("taskkill", {"/F", "/T", "/IM", "ffprobe.exe"});
					kill.waitForFinished();
					workerThread.waitForFinished();
				}
				break;
			case ENCODE:
				if (force) {
					kill.startDetached("taskkill", {"/F", "/T", "/IM", "ffmpeg.exe"});
				} else {
					kill.start("taskkill", {"/F", "/T", "/IM", "ffmpeg.exe"});
					kill.waitForFinished();
					workerThread.waitForFinished();

					if (bf::exists(settings->tempDir + "\\" + workerItem.name() + "." + settings->container)) {
						bf::remove(settings->tempDir + "\\" + workerItem.name() + "." + settings->container);
					}
				}
				break;
			default:

				break;
		}
	}
	return cancelWorker;
}

void MediaPreparer::lockUI(bool b) {
	if (!ui->setting_override->isChecked()) {
		ui->setting_vCodec->setEnabled(!b);
		ui->setting_vQuality->setEnabled(!b);
		ui->setting_aCodec->setEnabled(!b);
		ui->setting_aQuality->setEnabled(!b);
		ui->setting_subtitles->setEnabled(!b);
		ui->setting_threads->setEnabled(!b);
		ui->setting_extraParams->setEnabled(!b);
		ui->setting_forceEncode->setEnabled(!b);
		ui->setting_fixMetadata->setEnabled(!b);
	} else {
		ui->setting_override_args->setEnabled(!b);
	}
	ui->setting_preset->setEnabled(!b);
	ui->setting_directory->setEnabled(!b);
	ui->button_browse_directory->setEnabled(!b);
	ui->button_scan_directory->setEnabled(!b);
	ui->setting_dirOutput->setEnabled(!b);
	ui->button_browse_dirOutput->setEnabled(!b);
	ui->setting_container->setEnabled(!b);
	ui->setting_subdirectories->setEnabled(!b);
	ui->setting_subfolders->setEnabled(!b);
	ui->setting_override->setEnabled(!b);
	if (b) {
		ui->button_encode->setText("Cancel");
		ui->button_encode->setEnabled(true);
	}
}

void MediaPreparer::overrideUI(int b) {
	ui->setting_vCodec->setEnabled(!b);
	ui->setting_vQuality->setEnabled(!b);
	ui->setting_aCodec->setEnabled(!b);
	ui->setting_aQuality->setEnabled(!b);
	ui->setting_subtitles->setEnabled(!b);
	ui->setting_threads->setEnabled(!b);
	ui->setting_extraParams->setEnabled(!b);
	ui->setting_forceEncode->setEnabled(!b);
	ui->setting_fixMetadata->setEnabled(!b);
	ui->setting_override_args->setEnabled(b);
}

void MediaPreparer::blockSignals(bool b) {
	foreach (QWidget *x, ui->container_main->findChildren<QWidget *>()) { x->blockSignals(b); }
}

void MediaPreparer::closeEvent(QCloseEvent *e) {
	if (!workerThread.isRunning() || cancel()) {
		e->accept();
	} else {
		e->ignore();
	}
}

} // namespace SuperEpicFuntime::MediaPreparer
