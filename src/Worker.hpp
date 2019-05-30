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

namespace SuperEpicFuntime::MediaPreparer {
class Worker {
  private:
	WorkerType type;

	void worker_scan() {
		eventHandler->newEvent(WORKER_SCAN_STARTED, "Scanning Library: " + settings->libraryDir);
		eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, 0);
		library->scan();
		if (library->size() == 0) {
			eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, 1);
			eventHandler->newEvent(WORKER_SCAN_ERRORED, "Invalid Library: " + settings->libraryDir);
		} else {
			eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, library->size());
			for (int i = 0; !cancelWorker && i < library->size(); i++) {
				worker_scan_item(i);
			}
			library->scanEncode();

			if (cancelWorker) {
				eventHandler->newEvent(WORKER_SCAN_ERRORED, "Cancelled Scanning Library: " + settings->libraryDir);
			} else {
				eventHandler->newEvent(WORKER_SCAN_FINISHED, "Finished Scanning Library: " + settings->libraryDir, library->size());
			}
		}
	}

	void worker_scan_item(int i) {
		File &f = library->getFile(i);
		eventHandler->newEvent(WORKER_SCAN_ITEM_STARTED, "Scanning File: " + f.name(), i);
		QList<QString> params = {"-v", "quiet", "-show_entries", "format=duration:stream=codec_type:stream=codec_name", "-of", "json", f.path().c_str()};
		QProcess process;
		bool r = false;
		for (int j = 0; !cancelWorker && !r && j < RETRY_COUNT; j++) {
			process.start("ffprobe", params);
			process.waitForFinished();
			rapidjson::StringStream out(process.readAllStandardOutput());
			r = f.loadFileInfo(out);
		}
		eventHandler->newEvent(WORKER_SCAN_ITEM_FINISHED, i);
	}

	void worker_encode() {
		eventHandler->newEvent(WORKER_ENCODE_STARTED, "Encoding Library: " + settings->libraryDir);
		eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, 0);
		library->scanEncode();
		eventHandler->newEvent(PROGRESS_PRIMARY_MAXIMUM, library->sizeEncode());
		for (int i = 0; !cancelWorker && i < (int)library->sizeEncode(); i++) {
			worker_encode_item(i);
		}
		if (cancelWorker) {
			eventHandler->newEvent(WORKER_ENCODE_ERRORED, "Cancelled Encoding Library: " + settings->libraryDir);
		} else {
			eventHandler->newEvent(WORKER_ENCODE_FINISHED, "Finished Encoding Library: " + settings->libraryDir, library->sizeEncode());
		}
	}

	void worker_encode_item(int i) {
		File &f = library->getFileEncode(i);
		eventHandler->newEvent(WORKER_ENCODE_ITEM_STARTED, "Encoding File: " + f.name(), i);
		QList<QString> params = {"-y", "-v", "quiet", "-stats", "-hwaccel", "dxva2", "-threads", settings->threads.c_str(), "-i", f.path().c_str()};
		if (f.subtitles() == 1 && settings->subtitles.compare("Embed") == 0) {
			params += {"-i", f.pathSub().c_str()};
		}
		params += {"-map", "0:0", "-map", "0:1?"};
		if (f.subtitles() == 1 && settings->subtitles.compare("Embed") == 0) {
			params += {"-map", "1:0"};
		}
		if (f.subtitles() > 0 && settings->subtitles.compare("Remove") != 0) {
			params += {"-map", "0:2?", "-c:s", "srt", "-metadata:s:s:0", "language=eng", "-disposition:s:0", "default"};
		}
		params += {"-c:v", settings->vCodec.c_str(), "-crf", settings->vQuality.c_str(), "-c:a", settings->aCodec.c_str(), "-b:a", (settings->aQuality + "k").c_str()};
		if (!settings->extraParams.empty()) {
			char s[2048];
			strcpy(s, settings->extraParams.c_str());
			for (char *p = strtok(s, " "); p != NULL; p = strtok(NULL, " ")) {
				params.push_back(p);
			}
		}
		params += {"-metadata",
				   ("title=" + f.name()).c_str(),
				   "-metadata",
				   "comment=Processed by SuperEpicFuntime Media Preparer",
				   "-strict",
				   "-2",
				   (settings->tempDir + "\\" + f.name() + "." + settings->container).c_str()};
		QProcess process;
		process.start("ffmpeg", params);
		process.waitForFinished(-1);
		if (!cancelWorker) {
			boost::filesystem::rename(settings->tempDir + "\\" + f.name() + "." + settings->container, settings->outputDir + "\\" + f.name() + "." + settings->container);
		}
		eventHandler->newEvent(WORKER_ENCODE_ITEM_FINISHED, i);
	}

  public:
	Worker(WorkerType workerType) {
		type = workerType;
	}

	void run() {
		cancelWorker = false;
		if (eventHandler != NULL && settings != NULL && library != NULL) {
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
	}

	WorkerType getType() {
		return type;
	}
};
} // namespace SuperEpicFuntime
#endif // WORKER_HPP
