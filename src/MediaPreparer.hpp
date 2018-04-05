#ifndef MEDIAPREPARER_HPP
#define MEDIAPREPARER_HPP
#pragma once

#include <src/File.hpp>
#include <src/Library.hpp>
#include <src/Settings.hpp>

#include <boost/filesystem.hpp>

#include <QtWidgets/QAction>
#include <QtCore/QFuture>
#include <QtWidgets/QMenu>
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QTime>
#include <QtWidgets/QWidget>
#include <QtConcurrent/QtConcurrent>
#include <string>


namespace SuperEpicFuntime {
	namespace Ui {
		class MediaPreparer;
	}
	class MediaPreparer: public QWidget {
	Q_OBJECT
	private:
		Ui::MediaPreparer *ui;
		QSignalMapper *signalMapper = new QSignalMapper(this);
		QSystemTrayIcon *trayIcon;
		QMenu *trayIconMenu;
		QAction *minimizeAction;
		QAction *maximizeAction;
		QAction *restoreAction;
		QAction *quitAction;
		QTimer *updateTimer = new QTimer(this);
		QWidget *containerEncode;
		QWidget *containerEncodeList;

		QFuture<void> workerScan;
		QFuture<void> workerEncode;
		QFuture<void> workercleanUp;

		bool cancelScan = false;
		bool cancelEncode = false;

		QTime workerTimeStamp;
		QTime fileTimeStamp;

		int scanIndex;
		int encodeIndex;

		void closeEvent(QCloseEvent *);

		void scanLibrary(boost::filesystem::path directoryPath);

		void encodeLibrary();

		boost::filesystem::path getPath(std::string pathString);
		boost::filesystem::path getPath();

	public:
		SuperEpicFuntime::Settings *settings = new SuperEpicFuntime::Settings();
		SuperEpicFuntime::Library *library = new SuperEpicFuntime::Library();

		explicit MediaPreparer(QWidget *parent = 0);

		~MediaPreparer();

	private slots:
		void runWorkerScan();
		void workerScanStart();
		void workerScanAddItem(int itemPosition);
		void workerScanUpdateProgress(QString message, int progress);
		void workerScanUpdateProgress(QString message);
		void workerScanUpdateProgress(int progress);
		void workerScanEnd();

		void runWorkerEncode();
		void workerEncodeStart();
		void workerEncodeChangeItem(int itemPosition);
		void workerEncodeUpdateProgress(QString message, int progress);
		void workerEncodeUpdateProgress(QString message);
		void workerEncodeUpdateProgress(int progress);
		void workerEncodeEnd();

		void log(std::string message);
		void log(QString message);

		void Init();
		void InitGUI();

		void updateGUI();

		// void updateEncodeList();
		// void toggleEncodeFile(int position, int null = NULL);

		void encodeTab(bool display);
		void lockUIEncode(bool lock);
		void lockUILoad(bool lock);

		void createTrayActions();
		void createTrayIcon();
		void showTrayMessage(QString message);
		void trayIconActivated(QSystemTrayIcon::ActivationReason);

		void saveConfig();
		void loadConfig();
		bool checkConfig();

		bool diffPreset();
		void savePreset(std::string presetName);
		void savePreset();
		void loadPreset(std::string presetName);
		void loadPreset(QString presetName);

		void setEncodeOptions();

		void browseDialog(int type = 0);

		bool cancelDialog();
		bool cancel();

		void blockAllSignals(bool block);

		void on_button_encode_clicked();

	signals:
		void started_workerScan();
		void finished_workerScan();

		void started_workerEncode();
		void finished_workerEncode();

		void item_added_scan(int itemPosition);
		void progress_updated_scan(QString message, int progress);
		void progress_updated_scan(QString message);
		void progress_updated_scan(int progress);

		void item_changed_encode(int itemPosition);
		void progress_updated_encode(QString message, int progress);
		void progress_updated_encode(QString message);
		void progress_updated_encode(int progress);
	};
} // namespace SuperEpicFuntime
#endif // MEDIAPREPARER_HPP
