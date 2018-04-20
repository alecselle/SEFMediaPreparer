#ifndef WORKER_HPP
#define WORKER_HPP
#pragma once

#include <EventHandler.hpp>
#include <File.hpp>
#include <Global.hpp>

#include <QFuture>
#include <QTime>
#include <QtConcurrent/QtConcurrent>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <rapidjson/document.h>

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Worker
 */
class Worker {
  private:
	WorkerType type;
	QFuture<void> worker;
	QTime timeStamp;

	void run();

	void scanFile(File &file);
	void scanLibrary();

	void encodeFile(File &file);
	void encodeLibrary();

	void close();

  public:
	Worker(WorkerType type);
	~Worker();
};

} // namespace SuperEpicFuntime
#endif // WORKER_HPP