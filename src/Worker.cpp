#include "Worker.hpp"

namespace bf = boost::filesystem;
namespace bc = boost::container;
using namespace rapidjson;
using namespace std;

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Worker
 */
Worker::Worker(WorkerType t) {
	timeStamp = QTime();
	type = t;
	worker = QtConcurrent::run(this, &Worker::run);
}

void Worker::run() {
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
	eventHandler->addEvent(PROGRESS_UPDATED, "Scanning File: " + f.name(), PRIMARY);
	QProcess process;
	QStringList params;

	params.push_back("-v");
	params.push_back("quiet");
	params.push_back("-show_entries");
	params.push_back("format=duration:stream=codec_type:stream=codec_name");
	params.push_back("-of");
	params.push_back("json");
	params.push_back(f.path().c_str());

	process.setProgram("ffprobe");
	process.setArguments(params);

	process.start();
	process.waitForFinished();

	StringStream out(process.readAllStandardOutput());
	f.loadFileInfo(out);
}

void Worker::scanLibrary() {
	eventHandler->addEvent(WORKER_STARTED, "Scanning Library", SCAN);
	for (int i = 0; i < (int)library->size(); i++) {
		File &f = library->getFile(i);
		scanFile(f);
	}
	eventHandler->addEvent(WORKER_FINISHED, "Finished Scanning Library", SCAN);
}

/** ================================================================================================
 * (Section) Encode Worker
 */
void Worker::encodeFile(File &f) {
	eventHandler->addEvent(PROGRESS_UPDATED, "Scanning File: " + f.name(), PRIMARY);
	QProcess process;
	QStringList params;

	if (bf::exists((settings->outputDir + "\\" + f.name() + "." + settings->container).c_str())) {
		params.push_back("-y");
	}
	params +=
		{"-v", "quiet", "-stats", "-hwaccel", "dxva2", "-threads", settings->threads.c_str(), "-i", f.path().c_str()};
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
	process.setArguments(params);

	process.start();
	process.waitForFinished(-1);
}

void Worker::encodeLibrary() {
	eventHandler->addEvent(WORKER_STARTED, "Encoding Library", ENCODE);
	for (int i = 0; i < (int)library->sizeEncode(); i++) {
		File &f = library->getFileEncode(i);
		encodeFile(f);
	}
	eventHandler->addEvent(WORKER_FINISHED, "Finished Encoding Library", ENCODE);
}

/** ================================================================================================
 * (Section) Close Worker
 */
void Worker::close() {
}

} // namespace SuperEpicFuntime