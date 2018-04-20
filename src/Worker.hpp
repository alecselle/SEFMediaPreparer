#ifndef WORKER_HPP
#define WORKER_HPP
#pragma once

#include <EventHandler.hpp>
#include <File.hpp>
#include <Global.hpp>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QFuture>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTime>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <rapidjson/document.h>

namespace SuperEpicFuntime {
enum WorkerType { SCAN = 1, ENCODE = 2, CLOSE = 0 };

/** ================================================================================================
 * (Class) Worker
 */
class Worker : public QObject {
	Q_OBJECT
  private:
	WorkerType type;
	QTime timeStamp;

	void scanFile(File &file);
	void scanLibrary();

	void encodeFile(File &file);
	void encodeLibrary();

	void close();

  public:
	Worker();
	Worker(WorkerType type);
	~Worker();
  public slots:
	void process();
  signals:
	void finished();
};

} // namespace SuperEpicFuntime
#endif // WORKER_HPP