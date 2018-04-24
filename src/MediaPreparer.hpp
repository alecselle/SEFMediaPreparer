#ifndef MEDIAPREPARER_HPP
#define MEDIAPREPARER_HPP
#pragma once

#include <Global.hpp>

#include <EventHandler.hpp>
#include <File.hpp>
#include <Library.hpp>
#include <Settings.hpp>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>

#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stream.h>

#include <boost/bind.hpp>

#include <iostream> // For debugging (cout)

namespace SuperEpicFuntime {

namespace Ui {
class MediaPreparer;
}

/** ================================================================================================
 * (Class) MediaPreparer
 */
class MediaPreparer : public QWidget {
	Q_OBJECT
  public:
	Ui::MediaPreparer *ui;
	QSignalMapper *signalMapper = new QSignalMapper(this);

	QWidget *containerEncode;
	QWidget *containerEncodeList;

	const std::string productName = PRODUCT_NAME;
	const std::string productVersion = PRODUCT_VERSION;

	void init();
	void initGUI();
	void initSignals();

	void scanLibrary();

	void encodeLibrary();

	void closeEvent(QCloseEvent *e);

	QTimer *updateTimer = new QTimer(this);
	QFuture<void> worker;
	QTime workerTimeStamp;
	WorkerType workerType;
	File workerItem;
	QTime workerItemTimeStamp;
	bool cancelWorker = false;

	explicit MediaPreparer(QWidget *parent = 0);
	~MediaPreparer();

  public slots:
	void loadSettings_gui();
	void loadSettings_config();
	void loadSettings_preset(QString preset);
	void loadSettings_preset();
	void loadSettings_presets();

	void saveSettings_config();
	void saveSettings_preset(QString preset);
	void saveSettings_preset();

	void updateGUI_settings();
	void updateGUI_timers();

	void runWorker_scan();
	void runWorker_encode();
	void runWorker_cleanup();

	void eventListener(Event *);

	void dialogBrowse(int type = 0);
	void dialogSave();
	bool dialogCancel();

	bool cancel();

	void lockUI(bool b = true);
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

	void signal_dialogBrowse(int type = 0);
	void signal_dialogSave();
	void signal_dialogCancel();

	void signal_log(QString msg);
	void signal_blockSignals(bool b);
};

} // namespace SuperEpicFuntime
#endif // MEDIAPREPARER_HPP
