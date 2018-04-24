#include <Controller.hpp>

using std::string;

using namespace std; // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
Controller::Controller() {
	mediaPreparer = new MediaPreparer();
}

void Controller::show() {
	mediaPreparer->show();
}

} // namespace SuperEpicFuntime
