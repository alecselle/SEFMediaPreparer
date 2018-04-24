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
	WORKER_SCAN_STARTED = 00000101,
	WORKER_SCAN_FINISHED = 00000102,
	WORKER_SCAN_ERRORED = 00000103,
	WORKER_SCAN_ITEM_STARTED = 00000111,
	WORKER_SCAN_ITEM_FINISHED = 00000112,

	WORKER_ENCODE_STARTED = 00000201,
	WORKER_ENCODE_FINISHED = 00000202,
	WORKER_ENCODE_ERRORED = 00000203,
	WORKER_ENCODE_ITEM_STARTED = 00000211,
	WORKER_ENCODE_ITEM_FINISHED = 00000212,

	PROGRESS_PRIMARY_UPDATED = 00001101,
	PROGRESS_PRIMARY_MAXIMUM_CHANGED = 00001102,

	PROGRESS_SECONDARY_UPDATED = 00001201,
	PROGRESS_SECONDARY_MAXIMUM_CHANGED = 00001202,

	DIALOG_BROWSE = 00000770,
	DIALOG_SAVE = 00000771,
	DIALOG_ERROR = 00000777,

	CUSTOM = 9999,
	ERROR = -1
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
	Event(EventType type, std::string message = "", int data = -1, int error = 0);

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
	//	boost::container::vector<EventBinding> bindings;
	//	T *parent;

	boost::container::vector<Event *> events;

  public:
	//	EventHandler(T *parentObject) {
	//		parent = parentObject;
	//	}
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

	Event *getEvent(int pos = 0);
	int size();
  public slots:
	void newEvent(EventType type, std::string message, int data = -1, int error = 0);
	void newEvent(EventType type, int data = -1, int error = 0);

  signals:
	void eventAdded(Event *);
};

// static EventHandler<void, MediaPreparer> *eventHandler;
static EventHandler *eventHandler;

} // namespace SuperEpicFuntime
#endif // EVENTHANDLER_HPP
