#include "EventHandler.hpp"

#include <boost/container/vector.hpp>
#include <iostream>

namespace bc = boost::container;
using namespace std;

namespace SuperEpicFuntime {

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

EventHandler::EventHandler() {
	connect(this, SIGNAL(signal_addEvent(EventType, std::string, int)), this,
			SLOT(addEvent(EventType, std::string, int)));
	connect(this, SIGNAL(signal_addEvent(Event)), this, SLOT(addEvent(Event)));
}

void EventHandler::clearEvents() {
	events.clear();
}

void EventHandler::addEvent(EventType type, string message, int data) {
	events.insert(events.begin(), Event(type, message, data));
	emit eventAdded(getEvent());
}

void EventHandler::addEvent(Event event) {
	events.insert(events.begin(), event);
}

Event EventHandler::getEvent(int pos) {
	try {
		return events.at(pos);
	} catch (const std::out_of_range) {
		cout << "[ERROR] [OUT_OF_RANGE] getEvent(" << pos << ");" << endl;
		return Event(EventType::ERROR, "ERROR");
	}
}

Event EventHandler::getEvent() {
	return getEvent(0);
}

int EventHandler::size() {
	return events.size();
}
} // namespace SuperEpicFuntime