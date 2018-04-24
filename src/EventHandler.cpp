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
void EventHandler::newEvent(EventType t, std::string m, int d, int er) {
	Event *e = new Event(t, m, d, er);
	events.push_back(e);

	cout << "Event: " << (size() - 1) << " | ";
	cout << "Type : " << e->getType() << " | ";
	cout << "Error: " << e->getError() << " | ";
	cout << "Data : " << e->getData() << " | ";
	cout << "Mesg : " << e->getMessage() << std::endl;

	emit eventAdded(e);
	emit eventAdded(size() - 1);
}

void EventHandler::newEvent(EventType t, int d, int er) {
	newEvent(t, "", d, er);
}

Event *EventHandler::getEvent(int pos) {
	if (size() > 0) {
		if (pos == NULL) {
			return events[size() - 1];
		}
		if (pos >= 0 && pos < size()) {
			return events[pos];
		}
	}
	return new Event();
}

int EventHandler::size() {
	return events.size();
}

} // namespace SuperEpicFuntime
