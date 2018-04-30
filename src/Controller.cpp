#include <Controller.hpp>

using std::string;

using namespace std; // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
Controller::Controller() {

	eventHandler  = new EventHandler();
	settings	  = new Settings(eventHandler);
	library		  = new Library(settings, eventHandler);
	mediaPreparer = new MediaPreparer(eventHandler, settings, library);
}

void Controller::show() {
	mediaPreparer->show();
}

} // namespace SuperEpicFuntime
