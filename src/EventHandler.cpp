#include <EventHandler.hpp>

namespace bc = boost::container;
using std::string;

using namespace std; // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Event
 */
Event::Event() {
	type = ERROR;
	message = "ERROR";
	data = NULL;
	error = 1;
}
Event::Event(EventType t, std::string m, int d, int e) {
	type = t;
	message = m;
	data = d;
	error = e;
}
EventType Event::getType() {
	return type;
}
std::string Event::getMessage() {
	return message;
}
int Event::getData() {
	return data;
}
int Event::getError() {
	return error;
}

/** ================================================================================================
 * (Class) EventHandler
 */
EventHandler::EventHandler() {
	connect(this, SIGNAL(createEvent(EventType, std::string, int, int)), this, SLOT(newEvent(EventType, std::string, int, int)));
	connect(this, SIGNAL(createEvent(EventType, int, int)), this, SLOT(newEvent(EventType, int, int)));
}

void EventHandler::newEvent(EventType type, std::string message, int data, int error) {
	Event *e = new Event(type, message, data, error);
	events.insert(events.begin(), e);
	cout << "Event#: " << events.size() << " | Type: " << e->getType() << " | Error: " << e->getError() << " | Data: " << e->getData() << " | Message: " << e->getMessage() << endl;
	emit createdEvent(e);
	// bindings.push_back(EventFunction(type, &EventHandler::test, this));
	for (int i = 0; i < bindings.size(); i++) {
		if (bindings[i].getType() == type) {
			bindings[i]();
		}
	}
}
void EventHandler::newEvent(EventType type, int data, int error) {
	newEvent(type, "", data, error);
}

Event *EventHandler::getEvent(int pos) {
	if (pos >= 0 && pos < size()) {
		return events[pos];
	}
}

int EventHandler::size() {
	return events.size();
}

} // namespace SuperEpicFuntime
