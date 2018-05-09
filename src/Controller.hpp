#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <Event.hpp>
#include <Library.hpp>
#include <MediaPreparer.hpp>
#include <Settings.hpp>

#include <boost/filesystem.hpp>

#include <iostream> // For debugging (cout)

namespace SuperEpicFuntime::MediaPreparer {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
class Controller : public QObject {
	Q_OBJECT
  private:
	MediaPreparer *mediaPreparer;

  public:
	Controller();

	void show();

  public slots:
	void eventListener(Event *event);
	void log(QString message, bool toFile = true);
	void log(Event *event, bool toFile = true);
	void log(Event *event, std::string optMessage, bool toFile = true);
};

} // namespace SuperEpicFuntime
#endif // CONTROLLER_HPP
