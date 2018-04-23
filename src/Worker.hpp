#ifndef WORKER_HPP
#define WORKER_HPP
#pragma once

#include <EventHandler.hpp>
#include <Global.hpp>
#include <QtConcurrent/QtConcurrent>
#include <QtCore/QFuture>
#include <QtCore/QProcess>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/stream.h>
#include <string>

namespace SuperEpicFuntime {
enum WorkerType { NONE = 0, SCAN = 1, ENCODE = 2 };
class Worker : public QObject {
	Q_OBJECT
  private:
	WorkerType type;
	void scan();
	void encode();
	QFuture<void> thread;

  public:
	Worker(WorkerType type);
	void run();
};

} // namespace SuperEpicFuntime
#endif // WORKER_HPP