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

template <class S, class T> class EventFunction {
	typedef S (T::*Function)();

  private:
	EventType type;
	Function function;
	T *parent;

  public:
	EventFunction(EventType eventType, Function func, T *parentObject) {
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

/** ================================================================================================
 * (Class) EventHandler
 */
class EventHandler : public QObject {
	Q_OBJECT
  private:
	boost::container::vector<Event *> events;
	boost::container::vector<EventFunction<void, MediaPreparer>> bindings;

  public:
	EventHandler();

	void bind(EventType type, void (MediaPreparer::*f)(), MediaPreparer *parent) {
		bindings.push_back(EventFunction<void, MediaPreparer>(type, f, parent));
	}

	Event *getEvent(int pos = 0);

	int size();

	void test() {
		std::cout << "test" << std::endl;
	}

  public slots:
	void newEvent(EventType type, std::string message, int data = NULL, int error = 0);
	void newEvent(EventType type, int data = NULL, int error = 0);

  signals:
	void createdEvent(Event *);
	void createEvent(EventType type, std::string message, int data = NULL, int error = 0);
	void createEvent(EventType type, int data = NULL, int error = 0);
};

static EventHandler *eventHandler;

} // namespace SuperEpicFuntime
#endif // EVENTHANDLER_HPP
