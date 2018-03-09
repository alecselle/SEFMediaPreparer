#ifndef MEDIAPREPARER_H
#define MEDIAPREPARER_H

#include <File.h>
#include <Settings.h>
#include <VideoLibrary.h>

#include <boost/filesystem.hpp>

#include <QAction>
#include <QFuture>
#include <QMenu>
#include <QObject>
#include <QProcess>
#include <QSystemTrayIcon>
#include <QTime>
#include <QWidget>
#include <QtConcurrent/QtConcurrent>

namespace Ui {
class MediaPreparer;
}

class MediaPreparer : public QWidget {
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
	void runWorkerScan(boost::filesystem::path directoryPath);

	void encodeLibrary();
	void runWorkerEncode();

	void saveConfig();
	void loadConfig();
	bool checkConfig();

	bool diffPreset();
	void savePreset(std::string presetName);
	void savePreset();
	void loadPreset(std::string presetName);

	boost::filesystem::path getPath(std::string pathString);
	boost::filesystem::path getPath();

  public:
	Settings *settings = new Settings();
	VideoLibrary *library = new VideoLibrary();

	explicit MediaPreparer(QWidget *parent = 0);

	~MediaPreparer();

  private slots:
	void on_started_workerScan();
	void on_item_added(int itemPosition);
	void on_progress_updated(QString message, int progress);
	void on_progress_updated(QString message);
	void on_progress_updated(int progress);
	void on_finished_workerScan();

	void on_started_workerEncode();
	void on_item_changed_encode(int itemPosition);
	void on_progress_updated_encode(QString message, int progress);
	void on_progress_updated_encode(QString message);
	void on_progress_updated_encode(int progress);
	void on_finished_workerEncode();

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

	void setEncodeOptions();

	void browseDialog(int type = 0);

	bool cancelDialog();
	bool cancel();

	void blockAllSignals(bool block);

	void on_button_scan_directory_clicked();
	void on_setting_directory_returnPressed();

	void on_setting_preset_currentTextChanged(const QString &presetName);

	void on_button_savePreset_clicked();

	void on_button_encode_clicked();

  signals:
	void started_workerScan();
	void finished_workerScan();

	void started_workerEncode();
	void finished_workerEncode();

	void item_added(int itemPosition);
	void progress_updated(QString message, int progress);
	void progress_updated(QString message);
	void progress_updated(int progress);

	void item_changed_encode(int itemPosition);
	void progress_updated_encode(QString message, int progress);
	void progress_updated_encode(QString message);
	void progress_updated_encode(int progress);
};

#endif // MEDIAPREPARER_H
