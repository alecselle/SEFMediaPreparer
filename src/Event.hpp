#ifndef EVENT_HPP
#define EVENT_HPP
#pragma once

#include <SuperEpicFuntime/SEFLib.hpp>

#include <Global.hpp>
#include <QtCore/QObject>
#include <QtCore/QTime>
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/container/vector.hpp>

#include <iostream> // For debugging (cout)
using SuperEpicFuntime::SEFLib::Map, SuperEpicFuntime::SEFLib::Pair, std::string;

namespace SuperEpicFuntime::MediaPreparer {
enum EventType {
	INITIALIZED = 0x0000,
	TERMINATED = 0x0001,

	WORKER_SCAN_STARTED = 0xAA01,
	WORKER_SCAN_FINISHED = 0xAA02,
	WORKER_SCAN_ERRORED = 0xAA03,
	WORKER_SCAN_ITEM_STARTED = 0xAA11,
	WORKER_SCAN_ITEM_FINISHED = 0xAA12,

	WORKER_ENCODE_STARTED = 0xAB01,
	WORKER_ENCODE_FINISHED = 0xAB02,
	WORKER_ENCODE_ERRORED = 0xAB03,
	WORKER_ENCODE_ITEM_STARTED = 0xAB11,
	WORKER_ENCODE_ITEM_FINISHED = 0xAB12,

	PROGRESS_PRIMARY_UPDATED = 0xBA01,
	PROGRESS_PRIMARY_MAXIMUM = 0xBA02,

	PROGRESS_SECONDARY_UPDATED = 0xBB01,
	PROGRESS_SECONDARY_MAXIMUM = 0xBB02,

	CONFIG_SAVED = 0xCA01,
	CONFIG_LOADED = 0xCA02,

	PRESET_SAVED = 0xCB01,
	PRESET_LOADED = 0xCB02,

	CUSTOM = 0xAAAA,
	ERROR = 0xFFFF
};

static Map<int, string> EventTypeString = {Pair<int, string>(INITIALIZED, "INITIALIZED"),
										   Pair<int, string>(TERMINATED, "TERMINATED"),
										   Pair<int, string>(WORKER_SCAN_STARTED, "WORKER_SCAN_STARTED"),
										   Pair<int, string>(WORKER_SCAN_FINISHED, "WORKER_SCAN_FINISHED"),
										   Pair<int, string>(WORKER_SCAN_ERRORED, "WORKER_SCAN_ERRORED"),
										   Pair<int, string>(WORKER_SCAN_ITEM_STARTED, "WORKER_SCAN_ITEM_STARTED"),
										   Pair<int, string>(WORKER_SCAN_ITEM_FINISHED, "WORKER_SCAN_ITEM_FINISHED"),
										   Pair<int, string>(WORKER_ENCODE_STARTED, "WORKER_ENCODE_STARTED"),
										   Pair<int, string>(WORKER_ENCODE_FINISHED, "WORKER_ENCODE_FINISHED"),
										   Pair<int, string>(WORKER_ENCODE_ERRORED, "WORKER_ENCODE_ERRORED"),
										   Pair<int, string>(WORKER_ENCODE_ITEM_STARTED, "WORKER_ENCODE_ITEM_STARTED"),
										   Pair<int, string>(WORKER_ENCODE_ITEM_FINISHED, "WORKER_ENCODE_ITEM_FINISHED"),
										   Pair<int, string>(PROGRESS_PRIMARY_UPDATED, "PROGRESS_PRIMARY_UPDATED"),
										   Pair<int, string>(PROGRESS_PRIMARY_MAXIMUM, "PROGRESS_PRIMARY_MAXIMUM"),
										   Pair<int, string>(PROGRESS_SECONDARY_UPDATED, "PROGRESS_SECONDARY_UPDATED"),
										   Pair<int, string>(PROGRESS_SECONDARY_MAXIMUM, "PROGRESS_SECONDARY_MAXIMUM"),
										   Pair<int, string>(CONFIG_SAVED, "CONFIG_SAVED"),
										   Pair<int, string>(CONFIG_LOADED, "CONFIG_LOADED"),
										   Pair<int, string>(PRESET_SAVED, "PRESET_SAVED"),
										   Pair<int, string>(PRESET_LOADED, "PRESET_LOADED"),
										   Pair<int, string>(CUSTOM, "CUSTOM"),
										   Pair<int, string>(ERROR, "ERROR")};
static Map<string, int> StringEventType = EventTypeString.reverse();

static string parseEventType(int eventType) {
	return EventTypeString[eventType];
}
static EventType parseEventType(string eventTypeStr) {
	return static_cast<EventType>(StringEventType[eventTypeStr]);
}

/** ================================================================================================
 * (Class) Event
 */
class Event {
  private:
	const EventType type;
	std::string message {""};
	QTime timestamp {QTime::currentTime()};
	boost::container::vector<boost::any> data {};

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
		return EventTypeString[type];
	}

	boost::container::vector<boost::any> getDataVector() {
		return data;
	}

	int size() {
		return data.size();
	}

	boost::any getData(unsigned int i =0) {
		if (i < data.size()) {
			return data[i];
		}
	}

	template <typename T> T getData(unsigned int i = 0) {
		if (i < data.size()) {
			if (dataIsType<T>()) {
				return boost::any_cast<T>(data[i]);
			}
		}
	}

	template <typename T> bool dataIsType(unsigned int i = 0) {
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
	boost::container::vector<Event *> events {};

	void onEventAdded(Event *e) {
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

} // namespace SuperEpicFuntime::MediaPreparer
#endif // EVENT_HPP
