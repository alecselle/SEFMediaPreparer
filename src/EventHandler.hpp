#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP
#pragma once

#include <Global.hpp>

#include <QTime>
#include <QWidget>
#include <boost/container/vector.hpp>

namespace SuperEpicFuntime {

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
	void addEvent(EventType type, std::string message, int data = NULL);

  signals:
	void eventAdded(Event event);
	void signal_addEvent(EventType type, std::string message, int data = NULL);
};

/** ================================================================================================
 * (Class) EventListener
 */
class EventListener : public QObject {
	Q_OBJECT
  private:
  public:
  public slots:

  signals:
};

static EventHandler *eventHandler;

} // namespace SuperEpicFuntime
#endif // EVENTHANDLER_HPP