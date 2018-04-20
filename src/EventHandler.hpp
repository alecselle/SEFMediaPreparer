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
	WORKER_STARTED = 2,
	WORKER_FINISHED = 3,
	WORKER_ITEM_CHANGED = 4,
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

  public:
	EventHandler();

	void clearEvents();
	int size();
	Event getEvent(int pos = 0);

  public slots:
	void newEvent(EventType type, std::string message, int data = NULL);

  signals:
	void addedEvent(Event event);
	void addEvent(EventType type, std::string message, int data = NULL);
};

static EventHandler *eventHandler;

} // namespace SuperEpicFuntime
#endif // EVENTHANDLER_HPP