#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <QTime>
#include <QWidget>
#include <boost/container/vector.hpp>

namespace SuperEpicFuntime {
enum EventType { ERROR = 0, WORKER_STARTED = 1, WORKER_FINISHED = 2, PROGRESS_UPDATED = 3 };

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

class EventHandler : public QWidget {
	Q_OBJECT
  private:
	boost::container::vector<Event> events;

  public:
	EventHandler();

	void clearEvents();

	Event getEvent(int pos);
	Event getEvent();
	int size();
  public slots:
	void addEvent(EventType type, std::string message, int data = NULL);
	void addEvent(Event event);

  signals:
	void eventAdded(Event event);
	void signal_addEvent(EventType type, std::string message, int data = NULL);
	void signal_addEvent(Event event);
};
} // namespace SuperEpicFuntime
#endif // EVENTHANDLER_HPP