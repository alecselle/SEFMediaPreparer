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
	WORKER_CANCELED = 6,
	DIALOG_ERROR = 99,
	CUSTOM = 100
};
enum WorkerType { NONE = 0, SCAN = 1, ENCODE = 2 };

/** ================================================================================================
 * (Class) Event
 */
class Event : public QObject {
	Q_OBJECT
  private:
	EventType type;
	QTime timeStamp;
	std::string message;
	int data;
	int error;

  public:
	Event();
	Event(EventType type, std::string message, int data = NULL, int error = 0);
	EventType getType();
	QTime getTimeStamp();
	std::string getMessage();
	int getData();
	int getError();
};

/** ================================================================================================
 * (Class) EventHandler
 */
class EventHandler : public QObject {
	Q_OBJECT
  private:
	boost::container::vector<Event *> events;
	Event *current = new Event();
	Event *previous = new Event();

  public:
	EventHandler();

	Event *getEvent(int pos = 0);

	int size();

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