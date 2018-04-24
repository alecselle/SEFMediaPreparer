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

} // namespace SuperEpicFuntime
