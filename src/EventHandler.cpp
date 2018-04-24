#include <EventHandler.hpp>

namespace bc = boost::container;
using std::string;

using namespace std; // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Event
 */
Event::Event() {
	type	= ERROR;
	message = "ERROR";
	data	= -1;
	error   = 1;
}
Event::Event(EventType t, std::string m, int d, int e) {
	type	= t;
	message = m;
	data	= d;
	error   = e;
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
void EventHandler::newEvent(EventType type, std::string message, int data, int error) {
	Event *e = new Event(type, message, data, error);
	events.push_back(e);

	cout << "Event#: " << events.size() << " | ";
	cout << "Type: " << e->getType() << " | ";
	cout << "Error: " << e->getError() << " | ";
	cout << "Data: " << e->getData() << " | ";
	cout << "Message: " << e->getMessage() << std::endl;

	emit eventAdded(e);
}

void EventHandler::newEvent(EventType type, int data, int error) {
	newEvent(type, "", data, error);
}

Event *EventHandler::getEvent(int pos) {
	if (pos >= 0 && pos < size()) {
		return events[pos];
	}
	return new Event();
}

int EventHandler::size() {
	return events.size();
}

} // namespace SuperEpicFuntime
