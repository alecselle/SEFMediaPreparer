#ifndef MEDIAPREPARERGUI_HPP
#define MEDIAPREPARERGUI_HPP
#pragma once

#include <EventHandler.hpp>
#include <File.hpp>
#include <Global.hpp>
#include <Library.hpp>
#include <QtConcurrent/QtConcurrent>
#include <QtCore/QFuture>
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QThread>
#include <QtCore/QTime>
#include <QtCore>
#include <QtGui>
#include <QtWidgets/QAction>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <Settings.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <product_info.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stream.h>
#include <string>

namespace SuperEpicFuntime {

namespace Ui {
class MediaPreparer;
}

/** ================================================================================================
 * (Class) MediaPreparer
 */
class MediaPreparerGUI : public QWidget {
	Q_OBJECT
  private:
	Ui::MediaPreparer *ui;
	QSignalMapper *signalMapper = new QSignalMapper(this);
	QTimer *updateTimer = new QTimer(this);
	QWidget *containerEncode;
	QWidget *containerEncodeList;

	QFuture<void> worker;

	bool cancelScan = false;
	bool cancelEncode = false;

	const std::string productName = PRODUCT_NAME;
	const std::string productVersion = PRODUCT_VERSION;

	void init();
	void initGUI();
	void initSignals();

	void scanFile(File &file);
	void scanLibrary();

	void encodeFile(File &file);
	void encodeLibrary();

	void closeEvent(QCloseEvent *e);

  public:
	explicit MediaPreparerGUI(QWidget *parent = 0);
	~MediaPreparerGUI();

  public slots:
	void loadSettings_gui();
	void loadSettings_config();
	void loadSettings_preset(QString preset);
	void loadSettings_presets();

	void saveSettings_config();
	void saveSettings_preset(QString preset);

	void updateGUI_settings();
	void updateGUI_timers();

	void runWorker_scan();
	void runWorker_encode();
	void runWorker_cleanup();

	void eventListener(Event event);

	void updateProgress_primary(int progress = 0, QString msg = NULL);
	void updateProgress_secondary(int progress = 0, QString msg = NULL);

	void dialogBrowse(int type = 0);
	void dialogSave();
	void dialogCancel();

	void log(QString msg);
	void blockSignals(bool b);

  signals:
	void signal_loadSettings_gui();
	void signal_loadSettings_config();
	void signal_loadSettings_preset(QString preset);
	void signal_loadSettings_presets();

	void signal_saveSettings_config();
	void signal_saveSettings_preset(QString preset);

	void signal_updateGUI_settings();
	void signal_updateGUI_timers();

	void signal_runWorker_scan();
	void signal_runWorker_encode();
	void signal_runWorker_cleanup();

	void signal_updateProgress_primary(int progress = 0, QString msg = NULL);
	void signal_updateProgress_secondary(int progress = 0, QString msg = NULL);

	void signal_dialogBrowse(int type = 0);
	void signal_dialogSave();
	void signal_dialogCancel();

	void signal_log(QString msg);
	void signal_blockSignals(bool b);
};

} // namespace SuperEpicFuntime
#endif // MEDIAPREPARERGUI_HPP