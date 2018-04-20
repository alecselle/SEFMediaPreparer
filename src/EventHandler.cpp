#include "EventHandler.hpp"

namespace bc = boost::container;
using namespace std;

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Event
 */
Event::Event() {
	type = ERROR;
	timeStamp = QTime();
	message = "ERROR";
	data = NULL;
}

Event::Event(EventType t, string m, int d) {
	type = t;
	timeStamp = QTime();
	message = m;
	data = d;
}

EventType Event::getType() {
	return type;
}

QTime Event::getTimeStamp() {
	return timeStamp;
}

string Event::getMessage() {
	return message;
}

int Event::getData() {
	return data;
}

/** ================================================================================================
 * (Class) EventHandler
 */
EventHandler::EventHandler() {
}

void EventHandler::newEvent(EventType type, string message, int data) {
	Event *e = new Event(type, message, data);
	events.insert(events.begin(), e);
	emit createdEvent(e);
}

void EventHandler::newEvent(EventType type, int data) {
	newEvent(type, "", data);
}

Event *EventHandler::getEvent(int pos) {
	if (pos >= 0 && pos < size()) {
		return events[pos];
	}
}

Event *EventHandler::getEventPrevious() {
	return previous;
}

int EventHandler::size() {
	return events.size();
}

} // namespace SuperEpicFuntime