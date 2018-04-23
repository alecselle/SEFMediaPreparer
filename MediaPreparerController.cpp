#include <MediaPreparerController.hpp>

namespace SuperEpicFuntime {

MediaPreparerController::MediaPreparerController(int argc, char *argv[]) {
	this->argc = argc;
	this->argv = argv;
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
