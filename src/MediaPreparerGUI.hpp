#ifndef MEDIAPREPARERGUI_HPP
#define MEDIAPREPARERGUI_HPP
#pragma once

#include "src/File.hpp"
#include "src/Library.hpp"
#include "src/Settings.hpp"
#include "src/product_info.hpp"

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QFuture>
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QTime>
#include <QtWidgets/QAction>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QWidget>
#include <string>

namespace SuperEpicFuntime {
namespace Ui {
class MediaPreparer;
}
class MediaPreparerGUI : public QWidget {
	Q_OBJECT
  private:
	Ui::MediaPreparer *ui;
	QSignalMapper *signalMapper = new QSignalMapper(this);
	QTimer *updateTimer = new QTimer(this);
	QWidget *containerEncode;
	QWidget *containerEncodeList;

	QFuture<void> workerScan;
	QFuture<void> workerEncode;
	QFuture<void> workercleanUp;
	QTime workerTimeStamp;
	QTime fileTimeStamp;
	int scanIndex;
	int encodeIndex;
	bool cancelScan = false;
	bool cancelEncode = false;

	const std::string productName = PRODUCT_NAME;
	const std::string productVersion = PRODUCT_VERSION;

	void init();
	void initGUI();

	void loadSettings_config();
	void loadSettings_preset(std::string preset);

	void saveSettings_config();
	void saveSettings_preset(std::string preset);

	void closeEvent(QCloseEvent *e);

  private slots:
	void on_updateGUI_settings();
	void on_updateGUI_timers();

	void on_blockSignals(bool b);

  public:
	SuperEpicFuntime::Settings *settings = new SuperEpicFuntime::Settings();
	SuperEpicFuntime::Library *library = new SuperEpicFuntime::Library(settings);

	explicit MediaPreparerGUI(QWidget *parent = 0);
	~MediaPreparerGUI();

  public slots:
	void on_runWorker_scan();
	void on_runWorker_encode();
	void on_runWorker_cleanup();

	void on_updateProgress_primary(int progress = NULL, std::string msg = NULL);
	void on_updateProgress_secondary(int progress = NULL, std::string msg = NULL);

	void on_log(std::string msg);

  signals:
	void updateGUI_settings();
	void updateGUI_timers();

	void blockSignals(bool b);

	void runWorker_scan();
	void runWorker_encode();
	void runWorker_cleanup();

	void updateProgress_primary(int progress = NULL, std::string msg = NULL);
	void updateProgress_secondary(int progress = NULL, std::string msg = NULL);

	void log(std::string msg);
};

} // namespace SuperEpicFuntime
#endif // MEDIAPREPARERGUI_HPP