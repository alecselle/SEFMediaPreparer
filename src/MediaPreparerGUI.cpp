#include "src/MediaPreparerGUI.hpp"
#include "ui_MediaPreparer.h"

#include <QObject>
#include <QWidget>
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
using namespace std;

namespace SuperEpicFuntime {

MediaPreparerGUI::MediaPreparerGUI(QWidget *parent) : QWidget(parent), ui(new Ui::MediaPreparer) {
	ui->setupUi(this);
	init();
}

MediaPreparerGUI::~MediaPreparerGUI() {
}

void MediaPreparerGUI::init() {
	initGUI();
	initSignals();
}

void MediaPreparerGUI::initGUI() {
	emit signal_loadSettings_config();
	emit signal_loadSettings_preset(settings->presetPath);
}

void MediaPreparerGUI::initSignals() {
	connect(this, SIGNAL(signal_loadSettings_gui()), this, SLOT(loadSettings_gui()));
	connect(this, SIGNAL(signal_loadSettings_config), this, SLOT(loadSettings_config()));
	connect(this, SIGNAL(signal_loadSettings_preset(string)), this, SLOT(loadSettings_preset(string)));
	connect(this, SIGNAL(signal_saveSettings_config()), this, SLOT(saveSettings_config()));
	connect(this, SIGNAL(signal_saveSettings_preset(string)), this, SLOT(saveSettings_preset(string)));
	connect(this, SIGNAL(signal_updateGUI_settings()), this, SLOT(updateGUI_settings()));
	connect(this, SIGNAL(signal_updateGUI_timers()), this, SLOT(updateGUI_timers()));
	connect(this, SIGNAL(signal_runWorker_scan()), this, SLOT(runWorker_scan()));
	connect(this, SIGNAL(signal_runWorker_encode()), this, SLOT(runWorker_encode()));
	connect(this, SIGNAL(signal_runWorker_cleanup()), this, SLOT(runWorker_cleanup()));
	connect(this, SIGNAL(signal_updateProgress_primary(int, string)), this, SLOT(updateProgress_primary(int, string)));
	connect(this, SIGNAL(signal_updateProgress_secondary(int, string)), this,
			SLOT(updateProgress_secondary(int, string)));
	connect(this, SIGNAL(signal_log()), this, SLOT(log()));
	connect(this, SIGNAL(signal_blockSignals()), this, SLOT(blockSignals()));
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

	if (library->isValid()) {
		library->scanEncode();
	}
	ui->button_encode->setText(("Encode [" + std::to_string(library->sizeEncode()) + "]").c_str());
}

void MediaPreparerGUI::loadSettings_config() {
	blockSignals(true);
	ui->setting_vCodec->clear();
	ui->setting_aCodec->clear();
	ui->setting_container->clear();
	ui->setting_preset->clear();

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
	for (int i = 0; i < (int)settings->presetNameList.size(); i++) {
		ui->setting_preset->addItem(settings->presetNameList[i].c_str());
	}
	blockSignals(false);
}

void MediaPreparerGUI::loadSettings_preset(string preset) {
	blockSignals(true);
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
	blockSignals(false);
}

void MediaPreparerGUI::saveSettings_config() {
}

void MediaPreparerGUI::saveSettings_preset(string preset) {
}

void MediaPreparerGUI::updateGUI_settings() {
}

void MediaPreparerGUI::updateGUI_timers() {
}

void MediaPreparerGUI::runWorker_scan() {
}

void MediaPreparerGUI::runWorker_encode() {
}

void MediaPreparerGUI::runWorker_cleanup() {
}

void MediaPreparerGUI::updateProgress_primary(int progress, string msg) {
}

void MediaPreparerGUI::updateProgress_secondary(int progress, string msg) {
}

void MediaPreparerGUI::log(string msg) {
}

void MediaPreparerGUI::blockSignals(bool b) {
}

void MediaPreparerGUI::closeEvent(QCloseEvent *e) {
}

} // namespace SuperEpicFuntime