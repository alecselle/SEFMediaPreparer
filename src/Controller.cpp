#include <Controller.hpp>

using std::string;

using namespace std; // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
Controller::Controller() {
	mediaPreparer = new MediaPreparer();
	//	eventHandler  = new EventHandler();
	//	settings	  = new Settings(eventHandler);
	//	library		  = new Library(settings, eventHandler);
}

void Controller::show() {
	mediaPreparer->show();
}

} // namespace SuperEpicFuntime
