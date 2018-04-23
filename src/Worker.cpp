#include "Worker.hpp"

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
using namespace rapidjson;
using namespace std;

namespace SuperEpicFuntime {

Worker::Worker(WorkerType t) {
	type = t;
}

void Worker::run() {
	switch (type) {
		case SCAN: {
			scan();
			break;
		}
		case ENCODE: {
			encode();
			break;
		}
		default: { break; }
	}
}

/** ================================================================================================
 * (Section) Scan Worker
 */
void Worker::scan() {
	library->scan();
	eventHandler->newEvent(WORKER_STARTED, "Scanning Library", SCAN);
	eventHandler->newEvent(PROGRESS_MAXIMUM, library->size());
	for (int i = 0; !cancelWorker && i < library->size(); i++) {
		File &f = library->getFile(i);
		// eventHandler->newEvent(WORKER_ITEM_CHANGED, "SCAN", i);
		eventHandler->newEvent(WORKER_ITEM_STARTED, "Scanning File: " + f.name(), i);
		QList<QString> params = {"-v", "quiet", "-show_entries", "format=duration:stream=codec_type:stream=codec_name", "-of", "json", f.path().c_str()};
		QProcess process;
		bool r = false;
		for (int j = 0; !cancelWorker && !r && j < RETRY_COUNT; j++) {
			process.start("ffprobe", params);
			process.waitForFinished();
			StringStream out(process.readAllStandardOutput());
			r = f.loadFileInfo(out);
		}
		eventHandler->newEvent(WORKER_ITEM_FINISHED, i);
	}
	library->scanEncode();
	if (cancelWorker) {
		eventHandler->newEvent(WORKER_FINISHED, "Cancelled Scanning Library", SCAN, 2);
	} else {
		eventHandler->newEvent(WORKER_FINISHED, "Finished Scanning Library", SCAN);
	}
}

/** ================================================================================================
 * (Section) Encode Worker
 */
void Worker::encode() {
	library->scanEncode();
	eventHandler->newEvent(WORKER_STARTED, "Encoding Library", ENCODE);
	eventHandler->newEvent(PROGRESS_MAXIMUM, library->sizeEncode());
	for (int i = 0; !cancelWorker && i < (int)library->sizeEncode(); i++) {
		File &f = library->getFileEncode(i);
		eventHandler->newEvent(WORKER_ITEM_STARTED, "Encoding File: " + f.name(), i);
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
		eventHandler->newEvent(WORKER_ITEM_FINISHED, i);
	}
	if (cancelWorker) {
		eventHandler->newEvent(WORKER_FINISHED, "Cancelled Encoding Library", ENCODE, 2);
	} else {
		eventHandler->newEvent(WORKER_FINISHED, "Finished Encoding Library", ENCODE);
	}
}
} // namespace SuperEpicFuntime