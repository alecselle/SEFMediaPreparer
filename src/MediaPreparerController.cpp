#include <MediaPreparerController.hpp>

namespace SuperEpicFuntime {

MediaPreparerController::MediaPreparerController() {
}

MediaPreparerGUI *MediaPreparerController::getParent() {
	return p;
}

Ui::MediaPreparer *MediaPreparerController::getUI() {
	return ui;
}

void MediaPreparerController::show() {
	p->show();
}

} // namespace SuperEpicFuntime
