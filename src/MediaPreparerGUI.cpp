#include "src/MediaPreparerGUI.hpp"
#include "ui_MediaPreparer.h"

#include <QObject>
#include <QWidget>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>

namespace bf = boost::filesystem;
namespace bc = boost::container;
using namespace std;

namespace SuperEpicFuntime {

MediaPreparerGUI::MediaPreparerGUI(QWidget *parent) : QWidget(parent), ui(new Ui::MediaPreparer) {
	ui->setupUi(this);
	init();
}

MediaPreparerGUI::~MediaPreparerGUI() {
}

void MediaPreparerGUI::init() {
}
void MediaPreparerGUI::initGUI() {
}

void MediaPreparerGUI::loadSettings_config() {
	blockSignals(true);
	ui->setting_vCodec->clear();
	ui->setting_aCodec->clear();
	ui->setting_container->clear();

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
	blockSignals(false);
}

void MediaPreparerGUI::loadSettings_preset(std::string preset) {
}

void MediaPreparerGUI::saveSettings_config() {
}

void MediaPreparerGUI::saveSettings_preset(std::string preset) {
}

void MediaPreparerGUI::on_updateGUI_settings() {
}

void MediaPreparerGUI::on_updateGUI_timers() {
}

void MediaPreparerGUI::on_runWorker_scan() {
}

void MediaPreparerGUI::on_runWorker_encode() {
}

void MediaPreparerGUI::on_runWorker_cleanup() {
}

void MediaPreparerGUI::on_updateProgress_primary(int progress, std::string msg) {
}

void MediaPreparerGUI::on_updateProgress_secondary(int progress, std::string msg) {
}

void MediaPreparerGUI::on_log(std::string msg) {
}

void MediaPreparerGUI::on_blockSignals(bool b) {
}

void MediaPreparerGUI::closeEvent(QCloseEvent *e) {
}

} // namespace SuperEpicFuntime