#include <MediaPreparerController.hpp>
#include <MediaPreparerGUI.hpp>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	QWidget *widget = new SuperEpicFuntime::MediaPreparerGUI();
	SuperEpicFuntime::MediaPreparerController c(widget);

	c.show();

	return app.exec();
}
