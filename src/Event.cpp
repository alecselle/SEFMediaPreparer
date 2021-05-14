#include <Event.hpp>

namespace bc = boost::container;
namespace SuperEpicFuntime::MediaPreparer {

static std::string parseEventType(int eventType) {
	return EventTypeString[eventType];
}

static EventType parseEventType(std::string eventTypeStr) {
	return static_cast<EventType>(StringEventType[eventTypeStr]);
}

void Event::assignData(boost::container::vector<boost::any> d) {
	for (auto x : d) {
		if (x.type() == typeid(std::string) && message.empty()) {
			message = boost::any_cast<std::string>(x);
		} else if (x.type() == typeid(const char *) && message.empty()) {
			message = std::string(boost::any_cast<const char *>(x));
		} else if (x.type() == typeid(const char *)) {
			data.push_back(std::string(boost::any_cast<const char *>(x)));
		} else {
			data.push_back(x);
		}
	}
}

EventType Event::getType() {
	return type;
}

std::string Event::getTypeStr() {
	return EventTypeString[type];
}

boost::container::vector<boost::any> Event::getDataVector() {
	return data;
}

int Event::size() {
	return data.size();
}

boost::any Event::getData(unsigned int i) {
	if (i < data.size()) {
		return data[i];
	}
}

std::string Event::getMessage() {
	return message;
}

std::string Event::getTimeStamp() {
	return timestamp.toString("hh:mm:ss.zzz").toStdString();
}

void EventHandler::onEventAdded(Event *e) {
	emit eventAdded(e);
	emit eventAdded(size() - 1);
}

EventHandler::EventHandler() {}

Event *EventHandler::getEvent(int pos) {
	if (size() > 0) {
		if (pos == -1) {
			return events[size() - 1];
		}
		if (pos >= 0 && pos < size()) {
			return events[pos];
		}
	}
}

int EventHandler::size() {
	return events.size();
}

} // namespace SuperEpicFuntime::MediaPreparer
