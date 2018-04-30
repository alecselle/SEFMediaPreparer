#ifndef WORKER_HPP
#define WORKER_HPP

#include <Global.hpp>

#include <Event.hpp>

#include <Settings.hpp>

#include <File.hpp>
#include <Library.hpp>

#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stream.h>

#include <QtCore/QProcess>

namespace SuperEpicFuntime {
class Worker {
  private:
	WorkerType type;
	EventHandler *_eventHandler;
	Settings *_settings;
	Library *_library;

	bool cancelWorker;

	void worker_scan() {
		_eventHandler->newEvent(WORKER_SCAN_STARTED, "Scanning Library: " + _settings->libraryDir);
		_eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, 0);
		_library->scan();
		if (_library->size() == 0) {
			_eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, 1);
			_eventHandler->newEvent(WORKER_SCAN_ERRORED, "Invalid Library: " + _settings->libraryDir);
		} else {
			_eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, _library->size());
			for (int i = 0; !cancelWorker && i < _library->size(); i++) {
				File &f = _library->getFile(i);
				_eventHandler->newEvent(WORKER_SCAN_ITEM_STARTED, "Scanning File: " + f.name(), i);
				QList<QString> params = {"-v", "quiet", "-show_entries", "format=duration:stream=codec_type:stream=codec_name", "-of", "json", f.path().c_str()};
				QProcess process;
				bool r = false;
				for (int j = 0; !cancelWorker && !r && j < RETRY_COUNT; j++) {
					process.start("ffprobe", params);
					process.waitForFinished();
					rapidjson::StringStream out(process.readAllStandardOutput());
					r = f.loadFileInfo(out);
				}
				_eventHandler->newEvent(WORKER_SCAN_ITEM_FINISHED, i);
			}
			_library->scanEncode();

			if (cancelWorker) {
				_eventHandler->newEvent(WORKER_SCAN_ERRORED, "Cancelled Scanning Library: " + _settings->libraryDir);
			} else {
				_eventHandler->newEvent(WORKER_SCAN_FINISHED, "Finished Scanning Library: " + _settings->libraryDir, _library->size());
			}
		}
	}

	void worker_encode() {
	}

  public:
	Worker(WorkerType workerType) {
		type		  = workerType;
		_eventHandler = eventHandler;
		_settings	 = settings;
		_library	  = library;
	}

	void run() {
		switch (type) {
			case SCAN:
				worker_scan();
				break;
			case ENCODE:
				worker_encode();
				break;
			default:
				break;
		}
	}
};
} // namespace SuperEpicFuntime
#endif // WORKER_HPP