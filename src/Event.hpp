#ifndef EVENT_HPP
#define EVENT_HPP
#pragma once

#include <SuperEpicFuntime/SEFLib.hpp>

#include <Global.hpp>
#include <QtCore/QObject>
#include <QtCore/QTime>
#include <boost/any.hpp>
#include <boost/container/vector.hpp>

using SuperEpicFuntime::SEFLib::Map, SuperEpicFuntime::SEFLib::Pair;

namespace SuperEpicFuntime::MediaPreparer {

#define ENUM_SECTION {

/**
 * @brief Enumerations for event types
 */
enum EventType {
	INITIALIZED = 0x0000,
	TERMINATED = 0x0001,

	WORKER_SCAN_STARTED = 0xAA01,
	WORKER_SCAN_FINISHED = 0xAA02,
	WORKER_SCAN_ERRORED = 0xAA03,
	WORKER_SCAN_ITEM_STARTED = 0xAA11,
	WORKER_SCAN_ITEM_FINISHED = 0xAA12,

	WORKER_ENCODE_STARTED = 0xAB01,
	WORKER_ENCODE_FINISHED = 0xAB02,
	WORKER_ENCODE_ERRORED = 0xAB03,
	WORKER_ENCODE_ITEM_STARTED = 0xAB11,
	WORKER_ENCODE_ITEM_FINISHED = 0xAB12,

	PROGRESS_PRIMARY_UPDATED = 0xBA01,
	PROGRESS_PRIMARY_MAXIMUM = 0xBA02,

	PROGRESS_SECONDARY_UPDATED = 0xBB01,
	PROGRESS_SECONDARY_MAXIMUM = 0xBB02,

	CONFIG_SAVED = 0xCA01,
	CONFIG_LOADED = 0xCA02,

	PRESET_SAVED = 0xCB01,
	PRESET_LOADED = 0xCB02,

	CUSTOM = 0xAAAA,
	ERROR = 0xFFFF
};

// Pairs enumerations with strings
static Map<int, std::string> EventTypeString = {Pair<int, std::string>(INITIALIZED, "INITIALIZED"),
										   Pair<int, std::string>(TERMINATED, "TERMINATED"),
										   Pair<int, std::string>(WORKER_SCAN_STARTED, "WORKER_SCAN_STARTED"),
										   Pair<int, std::string>(WORKER_SCAN_FINISHED, "WORKER_SCAN_FINISHED"),
										   Pair<int, std::string>(WORKER_SCAN_ERRORED, "WORKER_SCAN_ERRORED"),
										   Pair<int, std::string>(WORKER_SCAN_ITEM_STARTED, "WORKER_SCAN_ITEM_STARTED"),
										   Pair<int, std::string>(WORKER_SCAN_ITEM_FINISHED, "WORKER_SCAN_ITEM_FINISHED"),
										   Pair<int, std::string>(WORKER_ENCODE_STARTED, "WORKER_ENCODE_STARTED"),
										   Pair<int, std::string>(WORKER_ENCODE_FINISHED, "WORKER_ENCODE_FINISHED"),
										   Pair<int, std::string>(WORKER_ENCODE_ERRORED, "WORKER_ENCODE_ERRORED"),
										   Pair<int, std::string>(WORKER_ENCODE_ITEM_STARTED, "WORKER_ENCODE_ITEM_STARTED"),
										   Pair<int, std::string>(WORKER_ENCODE_ITEM_FINISHED, "WORKER_ENCODE_ITEM_FINISHED"),
										   Pair<int, std::string>(PROGRESS_PRIMARY_UPDATED, "PROGRESS_PRIMARY_UPDATED"),
										   Pair<int, std::string>(PROGRESS_PRIMARY_MAXIMUM, "PROGRESS_PRIMARY_MAXIMUM"),
										   Pair<int, std::string>(PROGRESS_SECONDARY_UPDATED, "PROGRESS_SECONDARY_UPDATED"),
										   Pair<int, std::string>(PROGRESS_SECONDARY_MAXIMUM, "PROGRESS_SECONDARY_MAXIMUM"),
										   Pair<int, std::string>(CONFIG_SAVED, "CONFIG_SAVED"),
										   Pair<int, std::string>(CONFIG_LOADED, "CONFIG_LOADED"),
										   Pair<int, std::string>(PRESET_SAVED, "PRESET_SAVED"),
										   Pair<int, std::string>(PRESET_LOADED, "PRESET_LOADED"),
										   Pair<int, std::string>(CUSTOM, "CUSTOM"),
										   Pair<int, std::string>(ERROR, "ERROR")};
// Reverse enumeration/string pairs
static Map<std::string, int> StringEventType = EventTypeString.reverse();

/**
 * @brief Retrieves event string from int/EventType enum
 * @param eventType - EventType to retrieve string from
 * @return string corresponding to EventType
 */
static std::string parseEventType(int eventType);

/**
 * @brief Retieves EventType from string
 * @param eventTypeStr - String to retrieve EventType from
 * @return EventType corresponding to string
 */
static EventType parseEventType(std::string eventTypeStr);

/**
 * @brief Event container able to store any type of data
 * @author Alec S.
 */
class Event {
  private:
	// Event type
	const EventType type;
	// Event message
	std::string message {""};
	// Event timestamp
	QTime timestamp {QTime::currentTime()};
	// Event data
	boost::container::vector<boost::any> data {};

	void assignData(boost::container::vector<boost::any> d);

  public:
	template <typename... Args>
	/**
	 * @brief Template constructor, converts multiple of any data type
	 * @param type - EventType ID
	 * @param args - Any data, multiple types allowed
	 */
	Event(EventType type, Args... args) : type(type) {
		assignData({args...});
	}

	/**
	 * @brief Returns the type of event
	 * @return EventType
	 */
	EventType getType();

	/**
	 * @brief Returns the string corresponding to the type of event
	 * @return Event string
	 */
	std::string getTypeStr();

	/**
	 * @brief Returns the vector containing all stored data
	 * @return Vector of data
	 */
	boost::container::vector<boost::any> getDataVector();

	/**
	 * @brief Returns the number of data items stored
	 * @return Number of items
	 */
	int size();

	/**
	 * @brief Retrieves stored data
	 * @param i - Index of data to retrieve (default: 0)
	 * @return Unconverted data (boost::any)
	 */
	boost::any getData(unsigned int i = 0);

	template <typename T>
	/**
	 * @brief Attempts to retrieve stored data of a specified type
	 * @param T - Type of data
	 * @param i - Index of data to retrieve (default: 0)
	 * @return Converted data (nothing if type incorrect)
	 */
	T getData(unsigned int i = 0) {
		if (i < data.size()) {
			if (dataIsType<T>()) {
				return boost::any_cast<T>(data[i]);
			}
		}
	}

	template <typename T>
	/**
	 * @brief Checks if stored data matches a specified type
	 * @param T - Type to check
	 * @param i - Index of data to check (default: 0)
	 * @return True if type matches, False if not
	 */
	bool dataIsType(unsigned int i = 0) {
		if (i < data.size()) {
			if (typeid(int) == typeid(T) && data[i].type() == typeid(unsigned int)) return true;
			if (typeid(long) == typeid(T) && data[i].type() == typeid(unsigned long)) return true;
			return (data[i].type() == typeid(T));
		}
	}

	/**
	 * @brief Returns message
	 * @return Message string
	 */
	std::string getMessage();

	/**
	 * @brief Returns timestamp string
	 * @return Timestamp string
	 */
	std::string getTimeStamp();
};

/**
 * @brief Creates/stores events and sends qt signals
 * @author Alec S.
 */
class EventHandler : public QObject {
	Q_OBJECT
  private:
	// Event list
	boost::container::vector<Event *> events {};

	void onEventAdded(Event *e);

  public:
	EventHandler();

	template <typename... Args>
	/**
	 * @brief Template event creator, converts multiple of any data type
	 * @param type - EventType ID
	 * @param args - Any data, multiple types allowed
	 */
	void newEvent(EventType type, Args... args) {
		Event *e = new Event(type, args...);
		events.push_back(e);
		onEventAdded(e);
	}

	/**
	 * @brief Returns event at a specified index
	 * @footnote Default parameter is only to handle invalid calls from other parts of the program
	 * @param pos - Index of Event (REQUIRED to function)
	 * @return Event at index
	 */
	Event *getEvent(int pos = -1);

	/**
	 * @brief Returns number of events
	 * @return Number of events
	 */
	int size();

  signals:
	/**
	 * @brief Sends signal to other parts of the program
	 * @param event - New event
	 */
	void eventAdded(Event *event);

	/**
	 * @brief Sends signal to other parts of the program
	 * @param pos - Index of new event
	 */
	void eventAdded(int pos);
};

/**
 * @brief Static member for global events
 */
static EventHandler *eventHandler;

} // namespace SuperEpicFuntime::MediaPreparer
#endif // EVENT_HPP
