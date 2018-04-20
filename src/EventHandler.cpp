#include "EventHandler.hpp"

#include <boost/container/vector.hpp>
#include <iostream>

namespace bc = boost::container;
using namespace std;

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Event
 */
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

void EventHandler::addEvent(EventType type, string message, int data) {
	eventContainer.insert(eventContainer.begin(), Event(type, message, data));
	emit eventAdded(getEvent());
}

int EventHandler::size() {
	return eventContainer.size();
}

Event EventHandler::getEvent(int pos) {
	try {
		return eventContainer.at(pos);
	} catch (const std::out_of_range) {
		cout << "[ERROR] [OUT_OF_RANGE] getEvent(" << pos << ");" << endl;
		return Event(EventType::ERROR, "ERROR");
	}
}

/** ================================================================================================
 * (Class) EventListener
 */

} // namespace SuperEpicFuntime