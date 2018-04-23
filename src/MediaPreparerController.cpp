#include <MediaPreparerController.hpp>

using std::string;

using namespace std; // For debugging (cout)

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) MediaPreparerController
 */
MediaPreparerController::MediaPreparerController(QWidget *parent) {
	p = parent;
}

QWidget *MediaPreparerController::getParent() {
	return p;
}

void MediaPreparerController::show() {
	p->show();
}

} // namespace SuperEpicFuntime
