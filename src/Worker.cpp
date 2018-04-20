#include "Worker.hpp"

#include <iostream>

namespace bf = boost::filesystem;
namespace bc = boost::container;
using namespace rapidjson;
using namespace std;

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Worker
 */
Worker::Worker() {
}

Worker::Worker(WorkerType t) {
	cout << "yay" << endl;
	type = t;
}

Worker::~Worker() {
	cout << "fuq" << endl;
}

void Worker::process() {
	timeStamp = QTime();
	switch (type) {
	case SCAN:
		scanLibrary();
		break;
	case ENCODE:
		encodeLibrary();
		break;
	case CLOSE:
		close();
		break;
	}
}

/** ================================================================================================
 * (Section) Scan Worker
 */
void Worker::scanFile(File &f) {
	emit eventHandler->addEvent(PROGRESS_UPDATED, "Scanning File: " + f.name(), 1);
	emit eventHandler->addEvent(WORKER_ITEM_CHANGED, "SCAN", library->findFile(f));
	QProcess process;
	QList<QString> params = {"-v",  "quiet", "-show_entries", "format=duration:stream=codec_type:stream=codec_name",
							 "-of", "json",  f.path().c_str()};

	if (bf::exists("../lib/ffprobe.exe")) {
		process.setProgram("../lib/ffprobe");
	} else {
		process.setProgram("ffprobe");
	}

	process.setArguments((QStringList)params);

	process.start();
	process.waitForFinished();

	StringStream out(process.readAllStandardOutput());
	f.loadFileInfo(out);
}

void Worker::scanLibrary() {
	emit eventHandler->addEvent(WORKER_STARTED, "Scanning Library", SCAN);
	for (int i = 0; i < (int)library->size(); i++) {
		File &f = library->getFile(i);
		scanFile(f);
	}
	emit eventHandler->addEvent(WORKER_FINISHED, "Finished Scanning Library", SCAN);
}

/** ================================================================================================
 * (Section) Encode Worker
 */
void Worker::encodeFile(File &f) {
	emit eventHandler->addEvent(PROGRESS_UPDATED, "Encoding File: " + f.name(), 1);
	emit eventHandler->addEvent(WORKER_ITEM_CHANGED, "ENCODE", library->findFileEncode(f));
	QProcess process;
	QList<QString> params = {"-y",		 "-v",
							 "quiet",	"-stats",
							 "-hwaccel", "dxva2",
							 "-threads", settings->threads.c_str(),
							 "-i",		 f.path().c_str()};
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
	params += {"-c:v", settings->vCodec.c_str(), "-crf", settings->vQuality.c_str(),
			   "-c:a", settings->aCodec.c_str(), "-b:a", (settings->aQuality + "k").c_str()};
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

	process.setProgram("ffmpeg");
	process.setArguments((QStringList)params);

	process.start();
	process.waitForFinished(-1);
}

void Worker::encodeLibrary() {
	emit eventHandler->addEvent(WORKER_STARTED, "Encoding Library", ENCODE);
	for (int i = 0; i < (int)library->sizeEncode(); i++) {
		File &f = library->getFileEncode(i);
		encodeFile(f);
	}
	emit eventHandler->addEvent(WORKER_FINISHED, "Finished Encoding Library", ENCODE);
}

/** ================================================================================================
 * (Section) Close Worker
 */
void Worker::close() {
}

} // namespace SuperEpicFuntime