#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP
#pragma once

#include <Global.hpp>
#include <QtCore/QObject>
#include <boost/bind.hpp>
#include <boost/container/vector.hpp>
#include <functional>

#include <iostream> // For debugging (cout)

namespace SuperEpicFuntime {
enum EventType {
	ERROR = 0,
	PROGRESS_UPDATED = 1,
	PROGRESS_MAXIMUM = 2,
	WORKER_STARTED = 3,
	WORKER_FINISHED = 4,
	WORKER_ITEM_STARTED = 6,
	WORKER_ITEM_FINISHED = 7,
	DIALOG_ERROR = 99,
	CUSTOM = 100
};
class MediaPreparer;
/** ================================================================================================
 * (Class) Event
 */
class Event {
  private:
	EventType type;
	std::string message;
	int data;
	int error;

  public:
	Event();
	Event(EventType type, std::string message, int data = NULL, int error = 0);

	EventType getType();
	std::string getMessage();
	int getData();
	int getError();
};

/** ================================================================================================
 * (Class) EventHandler
 */
template <class S, class T> class EventHandler {
  private:
	typedef S (T::*Function)();

	class EventBinding {
	  private:
		EventType type;
		Function function;
		T *parent;

	  public:
		EventBinding(EventType eventType, Function func, T *parentObject) {
			type = eventType;
			function = func;
			parent = parentObject;
			std::cout << "created" << std::endl;
		}
		EventType getType() {
			return type;
		}
		std::function<T> getFunc() {
			return function;
		}
		S operator()() {
			(parent->*function)();
		}
	};

	boost::container::vector<Event *> events;
	boost::container::vector<EventBinding> bindings;
	T *parent;

  public:
	EventHandler(T *parentObject) {
		parent = parentObject;
	}

	void newEvent(EventType type, std::string message, int data = NULL, int error = 0) {
		Event *e = new Event(type, message, data, error);
		events.insert(events.begin(), e);
		std::cout << "Event#: " << events.size() << " | Type: " << e->getType() << " | Error: " << e->getError() << " | Data: " << e->getData() << " | Message: " << e->getMessage()
				  << std::endl;
		// bindings.push_back(EventFunction(type, &EventHandler::test, this));
		for (int i = 0; i < bindings.size(); i++) {
			if (bindings[i].getType() == type) {
				bindings[i]();
			}
		}
	}

	void newEvent(EventType type, int data = NULL, int error = 0) {
		newEvent(type, "", data, error);
	}

	void bind(EventType type, S (T::*f)()) {
		bindings.push_back(EventBinding(type, f, parent));
	}

	Event *getEvent(int pos = 0) {
		if (pos >= 0 && pos < size()) {
			return events[pos];
		}
	}

	int size() {
		return events.size();
	}

	void test() {
		std::cout << "test" << std::endl;
	}
};

// static EventHandler *eventHandler;

} // namespace SuperEpicFuntime
#endif // EVENTHANDLER_HPP
