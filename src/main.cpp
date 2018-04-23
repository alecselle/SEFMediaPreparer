#include <MediaPreparerController.hpp>
#include <MediaPreparerGUI.hpp>
#include <QtWidgets/QApplication>

using namespace SuperEpicFuntime;
// class SuperEpicFuntime::Event;

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	// SuperEpicFuntime::MediaPreparer w;
	// SuperEpicFuntime::MediaPreparerGUI w;
	MediaPreparerController c;

	c.show();

	return a.exec();
}
