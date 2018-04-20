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
	eventContainer.insert(eventContainer.begin(), Event(type, message, data));
	emit addedEvent(0);
}

void EventHandler::newEvent(EventType type, int data) {
	newEvent(type, "", data);
}

int EventHandler::size() {
	return eventContainer.size();
}

Event EventHandler::getEvent(int pos) {
	if (pos < size()) {
		lastProcessed = eventContainer.at(pos);
		return lastProcessed;
	}
	return Event();
}

Event EventHandler::getLastEvent() {
	return lastProcessed;
}

bool EventHandler::checkRepeat() {
	Event p = lastProcessed;
	Event e = eventContainer.at(0);
	return (e.getType() != p.getType() || e.getData() != p.getData());
}

} // namespace SuperEpicFuntime