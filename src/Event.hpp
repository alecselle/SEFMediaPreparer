#ifndef EVENT_HPP
#define EVENT_HPP
#pragma once

#include <Global.hpp>
#include <QtCore/QObject>
#include <QtCore/QTime>
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/container/vector.hpp>

#include <iostream> // For debugging (cout)

namespace SuperEpicFuntime {
enum EventType {
	INITIALIZED = 0x0000,
	TERMINATED  = 0x0001,

	WORKER_SCAN_STARTED		  = 0xAA01,
	WORKER_SCAN_FINISHED	  = 0xAA02,
	WORKER_SCAN_ERRORED		  = 0xAA03,
	WORKER_SCAN_ITEM_STARTED  = 0xAA11,
	WORKER_SCAN_ITEM_FINISHED = 0xAA12,

	WORKER_ENCODE_STARTED		= 0xAB01,
	WORKER_ENCODE_FINISHED		= 0xAB02,
	WORKER_ENCODE_ERRORED		= 0xAB03,
	WORKER_ENCODE_ITEM_STARTED  = 0xAB11,
	WORKER_ENCODE_ITEM_FINISHED = 0xAB12,

	PROGRESS_PRIMARY_UPDATED = 0xBA01,
	PROGRESS_PRIMARY_MAXIMUM = 0xBA02,

	PROGRESS_SECONDARY_UPDATED = 0xBB01,
	PROGRESS_SECONDARY_MAXIMUM = 0xBB02,

	CONFIG_SAVED  = 0xCA01,
	CONFIG_LOADED = 0xCA02,

	PRESET_SAVED  = 0xCB01,
	PRESET_LOADED = 0xCB02,

	CUSTOM = 0xAAAA,
	ERROR  = 0xFFFF
};

/** ================================================================================================
 * (Class) Event
 */
class Event {
  private:
	const EventType type;
	std::string message = "";
	QTime timestamp		= QTime::currentTime();
	boost::container::vector<boost::any> data;

	void assignData(boost::container::vector<boost::any> d) {
		for (auto x : d) {
			if (x.type() == typeid(std::string) && message.empty()) {
				message = boost::any_cast<std::string>(x);
			} else if (x.type() == typeid(const char *) && message.empty()) {
				message = std::string(boost::any_cast<const char *>(x));
			} else if (x.type() == typeid(const char *)) {
				data.push_back(std::string(boost::any_cast<const char *>(x)));
			} else {
				data.push_back(x);
			}
		}
	}

  public:
	template <typename... Args> Event(EventType type, Args... args) : type(type) {
		assignData({args...});
	}

	EventType getType() {
		return type;
	}

	std::string getTypeStr() {
		switch (type) {
			case INITIALIZED:
				return "INITIALIZED";
				break;
			case TERMINATED:
				return "TERMINATED";
				break;
			case WORKER_SCAN_STARTED:
				return "WORKER_SCAN_STARTED";
				break;
			case WORKER_SCAN_FINISHED:
				return "WORKER_SCAN_FINISHED";
				break;
			case WORKER_SCAN_ERRORED:
				return "WORKER_SCAN_ERRORED";
				break;
			case WORKER_SCAN_ITEM_STARTED:
				return "WORKER_SCAN_ITEM_STARTED";
				break;
			case WORKER_SCAN_ITEM_FINISHED:
				return "WORKER_SCAN_ITEM_FINISHED";
				break;
			case WORKER_ENCODE_STARTED:
				return "WORKER_ENCODE_STARTED";
				break;
			case WORKER_ENCODE_FINISHED:
				return "WORKER_ENCODE_FINISHED";
				break;
			case WORKER_ENCODE_ERRORED:
				return "WORKER_ENCODE_ERRORED";
				break;
			case WORKER_ENCODE_ITEM_STARTED:
				return "WORKER_ENCODE_ITEM_STARTED";
				break;
			case WORKER_ENCODE_ITEM_FINISHED:
				return "WORKER_ENCODE_ITEM_FINISHED";
				break;
			case PROGRESS_PRIMARY_UPDATED:
				return "PROGRESS_PRIMARY_UPDATED";
				break;
			case PROGRESS_PRIMARY_MAXIMUM:
				return "PROGRESS_PRIMARY_MAXIMUM";
				break;
			case PROGRESS_SECONDARY_UPDATED:
				return "PROGRESS_SECONDARY_UPDATED";
				break;
			case PROGRESS_SECONDARY_MAXIMUM:
				return "PROGRESS_SECONDARY_MAXIMUM";
				break;
			case CONFIG_SAVED:
				return "CONFIG_SAVED";
				break;
			case CONFIG_LOADED:
				return "CONFIG_LOADED";
				break;
			case PRESET_SAVED:
				return "PRESET_SAVED";
				break;
			case PRESET_LOADED:
				return "PRESET_LOADED";
				break;
			case CUSTOM:
				return "CUSTOM";
				break;
			case ERROR:
				return "ERROR";
				break;
			default:
				return "UNKNOWN";
		}
	}

	boost::container::vector<boost::any> getDataVector() {
		return data;
	}

	int size() {
		return data.size();
	}

	boost::any getData(int i = 0) {
		if (i < data.size()) {
			return data[i];
		}
	}

	template <typename T> T getData(int i = 0) {
		if (i < data.size()) {
			if (dataIsType<T>()) {
				return boost::any_cast<T>(data[i]);
			}
		}
	}

	template <typename T> bool dataIsType(int i = 0) {
		if (i < data.size()) {
			return (data[i].type() == typeid(T));
		}
	}

	std::string getMessage() {
		return message;
	}

	std::string getTimeStamp() {
		return timestamp.toString("hh:mm:ss.zzz").toStdString();
	}
};

/** ================================================================================================
 * (Class) EventHandler
 */
class EventHandler : public QObject {
	Q_OBJECT
  private:
	boost::container::vector<Event *> events;

	void onEventAdded(Event *e) {
		//		std::cout << "[" << e->getTimeStamp() << "]";
		//		std::cout << " Event: " << e->getTypeStr() << " [" << (size() - 1) << "]";
		//		for (int i = 0; i < e->getDataVector().size(); i++) {
		//			if (e->dataIsType<int>(i)) {
		//				std::cout << " | Data[" << i << "](int): " << e->getData<int>(i);
		//			} else if (e->dataIsType<std::string>(i)) {
		//				std::cout << " | Data[" << i << "](string): " << e->getData<std::string>(i);
		//			} else {
		//				std::cout << " | Data[" << i << "](" << e->getData(i).type().name() << "): Unknown";
		//			}
		//		}
		//		if (!e->getMessage().empty()) {
		//			std::cout << " | Mesg: " << e->getMessage();
		//		}
		//		std::cout << std::endl;

		emit eventAdded(e);
		emit eventAdded(size() - 1);
	}

  public:
	template <typename... Args> void newEvent(EventType type, Args... args) {
		Event *e = new Event(type, args...);
		events.push_back(e);
		onEventAdded(e);
	}

	Event *getEvent(int pos = -1) {
		if (size() > 0) {
			if (pos == -1) {
				return events[size() - 1];
			}
			if (pos >= 0 && pos < size()) {
				return events[pos];
			}
		}
	}

	int size() {
		return events.size();
	}

  signals:
	void eventAdded(Event *event);
	void eventAdded(int pos);
};

static EventHandler *eventHandler;

} // namespace SuperEpicFuntime
#endif // EVENT_HPP
