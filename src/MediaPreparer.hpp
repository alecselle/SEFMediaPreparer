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
	void scanFile(File &file);

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
	void blockSignals(bool b);
};

// static EventHandler<void, MediaPreparer> *eventHandler;

} // namespace SuperEpicFuntime
#endif // MEDIAPREPARER_HPP
