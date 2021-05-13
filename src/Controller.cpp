#include <Controller.hpp>

using std::string;
using namespace std;

namespace SuperEpicFuntime::MediaPreparer {

Controller::Controller() {

	eventHandler = new EventHandler();
	connect(eventHandler, SIGNAL(eventAdded(Event *)), this, SLOT(eventListener(Event *)), Qt::UniqueConnection);
	settings	  = new Settings(eventHandler);
	library		  = new Library(settings, eventHandler);
	mediaPreparer = new MediaPreparer(eventHandler, settings, library);
}

void Controller::show() {
	mediaPreparer->show();
}

void Controller::eventListener(Event *e) {
	EventType eventType = e->getType();
	string eventMessage = e->getMessage();
	switch (eventType) {
	case INITIALIZED:
		log(e, true);

		break;
	case TERMINATED:
		log(e, true);

		break;
	case WORKER_SCAN_STARTED: {
		log(e, true);
		break;
	}
	case WORKER_SCAN_FINISHED: {
		log(e, true);
		break;
	}
	case WORKER_SCAN_ERRORED: {
		log(e, true);
		break;
	}
	case WORKER_SCAN_ITEM_STARTED: {
		log(e, true);
		break;
	}
	case WORKER_SCAN_ITEM_FINISHED: {
		log(e, false);
		break;
	}
	case WORKER_ENCODE_STARTED: {
		log(e, true);
		break;
	}
	case WORKER_ENCODE_FINISHED: {
		log(e, true);
		break;
	}
	case WORKER_ENCODE_ERRORED: {
		log(e, true);
		break;
	}
	case WORKER_ENCODE_ITEM_STARTED: {
		log(e, true);
		break;
	}
	case WORKER_ENCODE_ITEM_FINISHED: {
		log(e, false);
		break;
	}
	case PROGRESS_PRIMARY_UPDATED: {
		log(e, false);
		break;
	}
	case PROGRESS_PRIMARY_MAXIMUM: {
		log(e, false);
		break;
	}
	case PROGRESS_SECONDARY_UPDATED: {
		log(e, false);
		break;
	}
	case PROGRESS_SECONDARY_MAXIMUM: {
		log(e, false);
		break;
	}
	case CONFIG_SAVED: {
		log(e, false);

		break;
	}
	case CONFIG_LOADED: {
		log(e, false);

		break;
	}
	case PRESET_SAVED: {
		log(e, true);

		break;
	}
	case PRESET_LOADED: {
		log(e, false);

		break;
	}
	case CUSTOM: {
		QStringList t = e->getData<QStringList>();
		for (int i {0}; i < t.size(); i++) {
			log (t[i], false);
		}
		break;
	}
	default: {
		log(e, "Unhandled Event", false);
		break;
	}
	}
}

void Controller::log(QString message, bool toFile) {
	boost::filesystem::fstream fs;
	if (toFile) {
		fs.open(settings->logPath, boost::filesystem::fstream::in | boost::filesystem::fstream::out | boost::filesystem::fstream::app);
	}

	std::string logStr = "[" + QTime::currentTime().toString("hh:mm:ss.zzz").toStdString() + "] " + message.toStdString();

	std::cout << logStr << endl;
	if (toFile) {
		fs << logStr << endl;
		fs.close();
	}
}

void Controller::log(Event *e, bool toFile) {
	log(e, "", toFile);
}

void Controller::log(Event *e, std::string optMessage, bool toFile) {
	boost::filesystem::fstream fs;
	if (toFile) {
		fs.open(settings->logPath, boost::filesystem::fstream::in | boost::filesystem::fstream::out | boost::filesystem::fstream::app);
	}

	std::string logStr = "[" + e->getTimeStamp() + "]";
	if (!optMessage.empty()) {
		logStr += " " + optMessage;
		if (!e->getMessage().empty()) {
			logStr += " |";
		}
	}
	if (!e->getMessage().empty()) {
		logStr += " " + e->getMessage();
	}
	logStr += " (Event: " + e->getTypeStr();
	for (unsigned int i {0}; i < e->getDataVector().size(); i++) {
		if (e->dataIsType<int>(i)) {
			logStr += ", Data[" + std::to_string(i) + "](int): " + std::to_string(e->getData<int>(i));
		} else if (e->dataIsType<std::string>(i)) {
			logStr += ", Data[" + std::to_string(i) + "](string): " + e->getData<std::string>(i);
		} else {
			logStr += ", Data[" + std::to_string(i) + "](" + e->getData(i).type().name() + "): Unknown";
		}
	}
	logStr += ")";

	std::cout << logStr << endl;
	if (toFile) {
		fs << logStr << endl;
		fs.close();
	}
}

} // namespace SuperEpicFuntime
