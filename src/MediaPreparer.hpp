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

namespace SuperEpicFuntime::MediaPreparer {

namespace Ui {
class MediaPreparer;
}

/**
 * @brief Main container class for program
 */
class MediaPreparer : public QMainWindow {
	Q_OBJECT
  public:
	const std::string productName	= PRODUCT_NAME;
	const std::string productVersion = PRODUCT_VERSION;
	// Main ui object
	Ui::MediaPreparer *ui;

	// Qt signal handler
	QSignalMapper *signalMapper = new QSignalMapper(this);
	// Timer to handle data refreshing
	QTimer *updateTimer {new QTimer(this)};

	// Thread for worker to run in
	QFuture<void> workerThread;
	// Primary worker
	Worker worker {NONE};
	// Timer to track worker runtime
	QElapsedTimer workerTimeStamp;
	// Type of worker running
	WorkerType workerType;
	// Item worker is working on
	File workerItem;
	// Timer to track worker runtime on current item
	QElapsedTimer workerItemTimeStamp;

	/**
	 * @brief Default constructor for program
	 * @param parent - Provided by Qt
	 */
	explicit MediaPreparer(QWidget *parent = nullptr);

	/**
	 * @brief Constructor using existing objects (static members)
	 * @param eventHandler - Existing EventHandler
	 * @param settings - Existing Settings
	 * @param library - Existing Library
	 * @param parent - Provided by Qt
	 */
	explicit MediaPreparer(EventHandler *eventHandler,
						   Settings *settings = nullptr,
						   Library *library = nullptr,
						   QWidget *parent = nullptr);

	/**
	 * @brief Initialize program
	 */
	void init();

	/**
	 * @brief Initialize gui
	 */
	void initGUI();

	/**
	 * @brief Initialize signal handling
	 */
	void initSignals();

	/**
	 * @brief Prompts user before exiting if worker is running
	 * @param e - Close event interrupted
	 */
	void closeEvent(QCloseEvent *e);

	/**
	 * @brief Deconstructor
	 */
	~MediaPreparer();

  public slots:
	/**
	 * @brief Load settings from input fields in gui
	 */
	void loadSettings_gui();

	/**
	 * @brief Load settings from Settings object
	 */
	void loadSettings_config();

	/**
	 * @brief Load settings from a preset file
	 * @param preset - Preset file path
	 */
	void loadSettings_preset(QString preset);

	/**
	 * @brief Load settings from a selected preset
	 */
	void loadSettings_preset();

	/**
	 * @brief Load list of presets available
	 */
	void loadSettings_presets();

	/**
	 * @brief Save settings to config file
	 */
	void saveSettings_config();

	/**
	 * @brief Save settings as a new preset
	 * @param preset - Name of new preset
	 */
	void saveSettings_preset(QString preset);

	/**
	 * @brief Save settings over an existing preset
	 */
	void saveSettings_preset();

	/**
	 * @brief Disables subtitle selection if container doesn't support embedding
	 */
	void updateGUI_settings_container();

	/**
	 * @brief Update timers in gui
	 */
	void updateGUI_timers();

	/**
	 * @brief Begins the scan worker
	 */
	void runWorker_scan();

	/**
	 * @brief Begins the encode worker
	 */
	void runWorker_encode();

	/**
	 * @brief Begins the cleanup worker
	 */
	void runWorker_cleanup();

	/**
	 * @brief Handles all events
	 */
	void eventListener(Event *);

	/**
	 * @brief Opens the browse dialog
	 * @param type - 0: Library Dir, 1: Output Dir
	 */
	void dialogBrowse(int type = 0);

	/**
	 * @brief Opens the save preset dialog
	 */
	void dialogSave();

	/**
	 * @brief Opens the exit confirmation dialog
	 * @return
	 */
	bool dialogCancel();

	/**
	 * @brief Stops the current worker and cleans up files
	 * @param force - Whether to force an immediate worker shutdown
	 * @return Whether worker was canceled
	 */
	bool cancel(bool force = false);

	/**
	 * @brief Disables input while worker is running
	 * @param b - Whether input should be disabled (default: true)
	 */
	void lockUI(bool b = true);

	/**
	 * @brief Disables most inputs when override box is checked
	 * @param b - Whether input should be disabled (default: true)
	 */
	void overrideUI(int b = 1);

	/**
	 * @brief Block all program signals
	 * @param b - Whether signals should be blocked
	 */
	void blockSignals(bool b);
};

} // namespace SuperEpicFuntime::MediaPreparer
#endif // MEDIAPREPARER_HPP
