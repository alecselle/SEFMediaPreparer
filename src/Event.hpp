#ifndef EVENT_HPP
#define EVENT_HPP
#pragma once

#include <Global.hpp>
#include <QtCore/QObject>
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/container/vector.hpp>

#include <iostream> // For debugging (cout)

namespace SuperEpicFuntime {
enum EventType {
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

	DIALOG_BROWSE = 0xFDA0,
	DIALOG_SAVE   = 0xFDB0,
	DIALOG_ERROR  = 0xFD00,

	CUSTOM = 0xFFFF,
	ERROR  = 0x0000
};

/** ================================================================================================
 * (Class) Event
 */
class Event {
  private:
	const EventType type;
	std::string message;
	boost::container::vector<boost::any> data;

	void assignData(boost::container::vector<boost::any> data) {
		for (auto x : data) {
			if (x.type() == typeid(std::string) && message.empty()) {
				message = boost::any_cast<std::string>(x);
			} else {
				data.push_back(x);
			}
		}
	}

  public:
	Event(EventType type, std::string message, boost::any data0, boost::any data1 = NULL, boost::any data2 = NULL, boost::any data3 = NULL) : type(type), message(message) {
		assignData({data0, data1, data2, data3});
	}

	Event(EventType type, boost::any data0, boost::any data1 = NULL, boost::any data2 = NULL, boost::any data3 = NULL) : type(type), message("") {
		assignData({data0, data1, data2, data3});
	}

	EventType getType() {
		return type;
	}

	boost::container::vector<boost::any> getDataVector() {
		return data;
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
			std::cout << data[i].type().name() << std::endl;
			return (data[i].type() == typeid(T));
		}
	}

	std::string getMessage() {
		return message;
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
		std::cout << "Event: " << (size() - 1) << " | ";
		std::cout << "Type: " << e->getType() << " | ";
		for (int i = 0; i < e->getDataVector().size(); i++) {
			if (e->dataIsType<int>(i)) {
				std::cout << "Data[" << i << "](int): " << e->getData<int>(i) << " | ";

			} else if (e->dataIsType<std::string>(i)) {
				std::cout << "Data[" << i << "](string): " << e->getData<std::string>(i) << " | ";
			} else {
				std::cout << "Data[" << i << "](NULL): Unknown/NULL | ";
			}
		}
		std::cout << "Mesg: " << e->getMessage() << std::endl;

		emit eventAdded(e);
		emit eventAdded(size() - 1);
	}

  public:
	void newEvent(EventType type, std::string message, boost::any data0, boost::any data1 = NULL, boost::any data2 = NULL, boost::any data3 = NULL) {
		Event *e = new Event(type, message, data0, data1, data2, data3);
		events.push_back(e);
		//		onEventAdded(e);
		emit eventAdded(e);
	}

	void newEvent(EventType type, boost::any data0, boost::any data1 = NULL, boost::any data2 = NULL, boost::any data3 = NULL) {
		Event *e = new Event(type, data0, data1, data2, data3);
		events.push_back(e);
		//		onEventAdded(e);
		emit eventAdded(e);
	}

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
