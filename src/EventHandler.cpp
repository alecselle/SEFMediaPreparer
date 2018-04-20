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

void EventHandler::clearEvents() {
	eventContainer.clear();
}

void EventHandler::newEvent(EventType type, string message, int data) {
	Event e = Event(type, message, data);
	if (size() == 0) {
		eventContainer.push_back(e);
		emit addedEvent(size() - 1);
	}
	Event p = eventContainer.at(size() - 1);
	if (!compare(e, p)) {
		eventContainer.push_back(e);
		emit addedEvent(size() - 1);
	}
}

void EventHandler::newEvent(EventType type, int data) {
	newEvent(type, "", data);
}

int EventHandler::size() {
	return eventContainer.size();
}

Event EventHandler::getEvent(int pos) {
	if (pos == -1) {
		pos = size() - 1;
	}
	if (pos >= 0 && pos < size()) {
		lastProcessed = eventContainer.at(pos);
		return lastProcessed;
	}
	return Event();
}

Event EventHandler::getLastEvent() {
	return lastProcessed;
}

bool EventHandler::compare(Event a, Event b) {
	return (a.getType() == b.getType() && a.getData() == b.getData() && a.getMessage().compare(b.getMessage()) == 0);
}

} // namespace SuperEpicFuntime