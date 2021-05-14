#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <Event.hpp>
#include <Library.hpp>
#include <MediaPreparer.hpp>
#include <Settings.hpp>

#include <boost/filesystem.hpp>
#include <iostream>

namespace SuperEpicFuntime::MediaPreparer {
/**
 * @brief Handles main display and events
 * @author Alec S.
 */
class Controller : public QObject {
	Q_OBJECT
  private:
	MediaPreparer *mediaPreparer;

  public:
	/**
	 * @brief Default constructor
	 */
	Controller();

	/**
	 * @brief Displays window
	 */
	void show();

  public slots:
	/**
	 * @brief Logs events
	 * @param event - Event to log
	 */
	void eventListener(Event *event);

	/**
	 * @brief Log QString to console and optionally to file
	 * @param message - Message to log
	 * @param toFile - Whether to log to file (default: true)
	 */
	void log(QString message, bool toFile = true);

	/**
	 * @brief Log event to console and optionally to file
	 * @param event - Event to log
	 * @param toFile - Whether to log to file (default: true)
	 */
	void log(Event *event, bool toFile = true);

	/**
	 * @brief Log event and extra message to console and optionally to file
	 * @param event - Event to log
	 * @param optMessage - Extra message
	 * @param toFile - Whether to log to file (default: true)
	 */
	void log(Event *event, std::string optMessage, bool toFile = true);
};

} // namespace SuperEpicFuntime::MediaPreparer
#endif // CONTROLLER_HPP
