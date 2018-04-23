#include <MediaPreparerController.hpp>

using std::string;

using namespace std; // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
MediaPreparerController::MediaPreparerController(QWidget *w) {
	widget = w;
}

QWidget *MediaPreparerController::getWidget() {
	return widget;
}

void MediaPreparerController::show() {
	widget->show();
}

} // namespace SuperEpicFuntime
