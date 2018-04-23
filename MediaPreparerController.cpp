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
	QApplication a(argc, argv);
	p->show();
}

} // namespace SuperEpicFuntime
