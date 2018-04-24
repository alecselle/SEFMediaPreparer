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
/*template <class S, class T> */ class EventHandler : public QObject {
	Q_OBJECT
  private:
	//	typedef S (T::*Function)();

	//	class EventBinding {
	//	  private:
	//		EventType type;
	//		int data;
	//		int error;
	//		Function function;
	//		T *parent;

	//	  public:
	//		EventBinding(EventType eventType, Function func, T *parentObject) {
	//			type = eventType;
	//			function = func;
	//			parent = parentObject;
	//			data = NULL;
	//			error = 0;
	//		}
	//		EventBinding(EventType eventType, int eventData, Function func, T *parentObject) {
	//			type = eventType;
	//			function = func;
	//			parent = parentObject;
	//			data = NULL;
	//			error = 0;
	//		}
	//		EventBinding(EventType eventType, int eventData, int eventError, Function func, T *parentObject) {
	//			type = eventType;
	//			function = func;
	//			parent = parentObject;
	//			data = NULL;
	//			error = eventError;
	//		}
	//		EventType getType() {
	//			return type;
	//		}
	//		int getData() {
	//			return data;
	//		}
	//		int getError() {
	//			return error;
	//		}
	//		std::function<T> getFunc() {
	//			return function;
	//		}
	//		S operator()() {
	//			(parent->*function)();
	//		}
	//	};

	boost::container::vector<Event *> events;
	//	boost::container::vector<EventBinding> bindings;
	//	T *parent;

  public:
	//	EventHandler(T *parentObject) {
	//		parent = parentObject;
	//	}
	EventHandler() {
	}

	Event *getEvent(int pos = 0) {
		if (pos >= 0 && pos < size()) {
			return events[pos];
		}
	}

	int size() {
		return events.size();
	}
  public slots:

	void newEvent(EventType type, std::string message, int data = NULL, int error = 0) {
		Event *e = new Event(type, message, data, error);
		events.insert(events.begin(), e);
		emit eventAdded(e);
		std::cout << "Event#: " << events.size() << " | Type: " << e->getType() << " | Error: " << e->getError() << " | Data: " << e->getData() << " | Message: " << e->getMessage()
				  << std::endl;

		//		callBindings(type, data, error);
	}

	void newEvent(EventType type, int data = NULL, int error = 0) {
		newEvent(type, "", data, error);
	}

	//	void bind(EventType type, S (T::*f)()) {
	//		bindings.push_back(EventBinding(type, f, parent));
	//	}

	//	void bind(EventType type, int data, S (T::*f)()) {
	//		bindings.push_back(EventBinding(type, data, f, parent));
	//	}

	//	void bind(EventType type, int data, int error, S (T::*f)()) {
	//		bindings.push_back(EventBinding(type, data, error, f, parent));
	//	}

	//	void callBindings(EventType type, int data, int error) {
	//		for (int i = 0; i < bindings.size(); i++) {
	//			auto binding = bindings[i];
	//			if (binding.getType() == type && (binding.getData() == data || binding.getData() == NULL) && (binding.getError() == error)) {
	//				bindings[i]();
	//			}
	//		}
	//	}

  signals:
	void eventAdded(Event *);
};

// static EventHandler<void, MediaPreparer> *eventHandler;
static EventHandler *eventHandler;

} // namespace SuperEpicFuntime
#endif // EVENTHANDLER_HPP
