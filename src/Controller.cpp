#include <Controller.hpp>

using std::string;

using namespace std; // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
Controller::Controller() {
	mediaPreparer = new MediaPreparer();
	eventHandler = new EventHandler<void, MediaPreparer>(mediaPreparer);
	settings = new Settings();
	library = new Library(settings);
}

void Controller::show() {
	mediaPreparer->show();
}

} // namespace SuperEpicFuntime
