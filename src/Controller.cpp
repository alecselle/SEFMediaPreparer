#include <Controller.hpp>

using std::string;

using namespace std; // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
Controller::Controller(QWidget *w) {
	widget = w;
}

Controller::Controller() {
	widget = new MediaPreparer();
}

QWidget *Controller::getWidget() {
	return widget;
}

void Controller::show() {
	widget->show();
}

} // namespace SuperEpicFuntime
