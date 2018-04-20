#include "src/MediaPreparerGUI.hpp"
#include "ui_MediaPreparer.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/stream.h>
#include <string>

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
	saveSettings_config();
}

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
	connect(this, SIGNAL(signal_updateProgress_primary(int, QString)), this,
			SLOT(updateProgress_primary(int, QString)));
	connect(this, SIGNAL(signal_updateProgress_secondary(int, QString)), this,
			SLOT(updateProgress_secondary(int, QString)));
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

	// connect(new EventHandler, SIGNAL(addedEvent(Event)), this, SLOT(eventListener(Event)));
	connect(eventHandler, SIGNAL(addedEvent(Event)), this, SLOT(eventListener(Event)));
}

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

void MediaPreparerGUI::saveSettings_config() {
	loadSettings_gui();
	settings->saveConfig();
}

void MediaPreparerGUI::saveSettings_preset(QString preset) {
	loadSettings_gui();
	settings->savePresetAs(preset.toStdString());
	loadSettings_preset(preset);
}

void MediaPreparerGUI::updateGUI_settings() {
}

void MediaPreparerGUI::updateGUI_timers() {
}

void MediaPreparerGUI::runWorker(WorkerType t) {
	cout << "Worker Scanning " << library->size() << endl;
	emit eventHandler->addEvent(WORKER_STARTED, "Scanning Library", SCAN);
	library->scan();
	for (int i = 0; i < (int)library->size(); i++) {
		File &f = library->getFile(i);
		emit eventHandler->addEvent(PROGRESS_UPDATED, "Scanning File: " + f.name(), 1);
		emit eventHandler->addEvent(WORKER_ITEM_CHANGED, "SCAN", library->findFile(f));
		QProcess process;
		QList<QString> params = {"-v",  "quiet", "-show_entries", "format=duration:stream=codec_type:stream=codec_name",
								 "-of", "json",  f.path().c_str()};

		if (bf::exists("../lib/ffprobe.exe")) {
			process.setProgram("../lib/ffprobe");
		} else {
			process.setProgram("ffprobe");
		}

		process.setArguments((QStringList)params);

		process.start();
		process.waitForFinished();

		StringStream out(process.readAllStandardOutput());
		f.loadFileInfo(out);
	}
	library->scanEncode();
	emit eventHandler->addEvent(WORKER_FINISHED, "Finished Scanning Library", SCAN);
}

void MediaPreparerGUI::runWorker_scan() {
	runWorker(SCAN);
}

void MediaPreparerGUI::runWorker_encode() {
	runWorker(ENCODE);
}

void MediaPreparerGUI::runWorker_cleanup() {
	runWorker(CLOSE);
}

void MediaPreparerGUI::eventListener(Event e) {
	cout << e.getType() << endl;
	cout << e.getData() << endl;
	cout << e.getMessage() << endl;

	EventType t = e.getType();
	QTime ts = e.getTimeStamp();
	string m = e.getMessage();
	int d = e.getData();
	File f;

	switch (t) {
	case PROGRESS_UPDATED:
		updateProgress_primary(d, m.c_str());
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
}

void MediaPreparerGUI::updateProgress_primary(int progress, QString msg) {
}

void MediaPreparerGUI::updateProgress_secondary(int progress, QString msg) {
}

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

void MediaPreparerGUI::log(QString msg) {
}

void MediaPreparerGUI::blockSignals(bool b) {
	QList<QWidget *> widgets = ui->container_main->findChildren<QWidget *>();
	foreach (QWidget *x, widgets) { x->blockSignals(b); }
}

void MediaPreparerGUI::closeEvent(QCloseEvent *e) {
}

} // namespace SuperEpicFuntime