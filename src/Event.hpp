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
	EventType type;
	boost::container::vector<boost::any> data;
	boost::any d;
	std::string message;

	void charToString() {
		for (int i = 0; i < data.size(); i++) {
			if (data[i].type() == typeid(const char *)) {
				data[i] = (std::string)boost::any_cast<const char *>(data[i]);
			}
		}
	}

  public:
	Event(EventType type, std::initializer_list<boost::any> data, std::string message = "") {
		this->type	= type;
		this->data	= data;
		this->message = message;
		charToString();
	}

	Event(EventType type, boost::any data, std::string message = "") {
		this->type = type;
		this->data.insert(this->data.begin(), data);
		this->message = message;
		charToString();
	}

	Event(EventType type, boost::any d) : type(type), d(d) {
	}

	EventType getType() {
		return type;
	}

	boost::container::vector<boost::any> &getDataVector() {
		return data;
	}

	boost::any &getData(int i = 0) {
		if (i < data.size()) {
			return data[i];
		}
	}

	template <class T> T getData(int i = 0) {
		if (i < data.size() && dataIsType<T>()) {
			return boost::any_cast<T>(data[i]);
		}
	}

	template <class T> bool dataIsType(int i = 0) {
		if (i < data.size()) {
			return (data[i].type() == typeid(T));
		}
	}

	std::string &getMessage() {
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
		std::cout << "Type : " << e->getType() << " | ";
		for (int i = 0; i < e->getDataVector().size(); i++) {
			if (e->dataIsType<int>(i)) {
				int data = boost::any_cast<int>(e->getData(i));
				std::cout << "Data[" << i << "](int) : " << data << " | ";
			} else if (e->dataIsType<std::string>(i)) {
				std::string data = boost::any_cast<std::string>(e->getData(i));
				std::cout << "Data[" << i << "](string) : " << data << " | ";
			} else {
				std::cout << "Data[" << i << "](NULL) : Unknown/NULL | ";
			}
		}
		std::cout << "Mesg : " << e->getMessage() << std::endl;

		emit eventAdded(getEvent());
		emit eventAdded(size() - 1);
	}

  public:
	void newEvent(EventType type, boost::any data, std::string message = "") {
		Event *e = new Event(type, data, message);
		events.push_back(e);
		onEventAdded(e);
	}

	void newEvent(EventType type, std::initializer_list<boost::any> data) {
		Event *e = new Event(type, data);
		events.push_back(e);
		onEventAdded(e);
	}

	Event *getEvent(int pos = NULL) {
		if (size() > 0) {
			if (pos == NULL) {
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
