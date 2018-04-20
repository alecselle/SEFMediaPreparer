#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP
#pragma once

#include <Global.hpp>
#include <QtCore/QTime>
#include <QtWidgets/QWidget>
#include <boost/container/vector.hpp>
#include <iostream>
#include <string>

namespace SuperEpicFuntime {
enum EventType {
	ERROR = 0,
	PROGRESS_UPDATED = 1,
	PROGRESS_MAXIMUM = 2,
	WORKER_STARTED = 3,
	WORKER_FINISHED = 4,
	WORKER_ITEM_CHANGED = 5,
	DIALOG_ERROR = 99
};
enum WorkerType { SCAN = 1, ENCODE = 2, CLOSE = 0 };

/** ================================================================================================
 * (Class) Event
 */
class Event {
  private:
	EventType type;
	QTime timeStamp;
	std::string message;
	int data;

  public:
	Event();
	Event(EventType type, std::string message, int data = NULL);
	EventType getType();
	QTime getTimeStamp();
	std::string getMessage();
	int getData();
};

/** ================================================================================================
 * (Class) EventHandler
 */
class EventHandler : public QObject {
	Q_OBJECT
  private:
	boost::container::vector<Event> eventContainer;
	Event lastProcessed = Event();

  public:
	EventHandler();

	void clearEvents();
	int size();
	Event getEvent(int pos = -1);
	Event getLastEvent();
	static bool compare(Event a, Event b);

  public slots:
	void newEvent(EventType type, std::string message, int data = NULL);
	void newEvent(EventType type, int data = NULL);

  signals:
	void addedEvent(int pos);
	void addEvent(EventType type, std::string message, int data = NULL);
	void addEvent(EventType type, int data = NULL);
};

static EventHandler *eventHandler;

} // namespace SuperEpicFuntime
#endif // EVENTHANDLER_HPP