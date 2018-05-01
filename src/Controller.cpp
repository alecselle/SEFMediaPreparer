#include <Controller.hpp>

using std::string;

using namespace std; // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
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
		/** ============================================================================================
		 * (Event) INITIALIZED
		 */
		case INITIALIZED:
			log(e, true);

			break;
		/** ============================================================================================
		 * (Event) TERMINATED
		 */
		case TERMINATED:
			log(e, true);

			break;
		/** ============================================================================================
		 * (Event) WORKER_SCAN_STARTED
		 */
		case WORKER_SCAN_STARTED: {
			log(e, true);
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_SCAN_FINISHED
		 */
		case WORKER_SCAN_FINISHED: {
			log(e, true);
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_SCAN_ERRORED
		 */
		case WORKER_SCAN_ERRORED: {
			log(e, true);
			break;
		}
		/** ================================================================================================
		 * (Event) WORKER_SCAN_ITEM_STARTED
		 */
		case WORKER_SCAN_ITEM_STARTED: {
			log(e, true);
			break;
		}
		/** ================================================================================================
		 * (Event) WORKER_SCAN_ITEM_FINISHED
		 */
		case WORKER_SCAN_ITEM_FINISHED: {
			log(e, false);
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_ENCODE_STARTED
		 */
		case WORKER_ENCODE_STARTED: {
			log(e, true);
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_ENCODE_FINISHED
		 */
		case WORKER_ENCODE_FINISHED: {
			log(e, true);
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_ENCODE_ERRORED
		 */
		case WORKER_ENCODE_ERRORED: {
			log(e, true);
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_ENCODE_ITEM_STARTED
		 */
		case WORKER_ENCODE_ITEM_STARTED: {
			log(e, true);
			break;
		}
		/** ============================================================================================
		 * (Event) WORKER_ENCODE_ITEM_FINISHED
		 */
		case WORKER_ENCODE_ITEM_FINISHED: {
			log(e, false);
			break;
		}
		/** ============================================================================================
		 * (Event) PROGRESS_PRIMARY_UPDATED
		 */
		case PROGRESS_PRIMARY_UPDATED: {

			break;
		}
		/** ============================================================================================
		 * (Event) PROGRESS_PRIMARY_MAXIMUM_CHANGED
		 */
		case PROGRESS_PRIMARY_MAXIMUM: {

			break;
		}
		/** ============================================================================================
		 * (Event) CONFIG_SAVED
		 */
		case CONFIG_SAVED: {
			log(e, false);

			break;
		}
		/** ============================================================================================
		 * (Event) CONFIG_LOADED
		 */
		case CONFIG_LOADED: {
			log(e, false);

			break;
		}
		/** ============================================================================================
		 * (Event) PRESET_SAVED
		 */
		case PRESET_SAVED: {
			log(e, true);

			break;
		}
		/** ============================================================================================
		 * (Event) PRESET_LOADED
		 */
		case PRESET_LOADED: {
			log(e, false);

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
	for (int i = 0; i < e->getDataVector().size(); i++) {
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
