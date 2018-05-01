#ifndef MEDIAPREPARER_HPP
#define MEDIAPREPARER_HPP
#pragma once

#include <Global.hpp>
#include <Settings.hpp>

#include <Event.hpp>
#include <File.hpp>
#include <Library.hpp>
#include <Worker.hpp>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>

#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>
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
class MediaPreparer : public QMainWindow {
	Q_OBJECT
  public:
	Ui::MediaPreparer *ui;
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *helpMenu;

	QSignalMapper *signalMapper = new QSignalMapper(this);

	QWidget *containerEncode;
	QWidget *containerEncodeList;

	const std::string productName	= PRODUCT_NAME;
	const std::string productVersion = PRODUCT_VERSION;

	void init();
	void initGUI();
	void initSignals();

	void closeEvent(QCloseEvent *e);

	QTimer *updateTimer = new QTimer(this);
	QFuture<void> workerThread;
	Worker worker = Worker(NONE);
	QTime workerTimeStamp;
	WorkerType workerType;
	File workerItem;
	QTime workerItemTimeStamp;

	explicit MediaPreparer(QWidget *parent = 0);
	explicit MediaPreparer(EventHandler *eventHandler, Settings *settings = NULL, Library *library = NULL, QWidget *parent = 0);
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

	bool cancel(bool force = false);

	void lockUI(bool b = true);
	void blockSignals(bool b);
};

// static EventHandler<void, MediaPreparer> *eventHandler;

} // namespace SuperEpicFuntime
#endif // MEDIAPREPARER_HPP
