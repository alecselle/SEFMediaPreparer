#include <QtWidgets/QApplication>
#include <src/MediaPreparer.hpp>
#include <src/MediaPreparerGUI.hpp>

class SuperEpicFuntime::Event;

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	// SuperEpicFuntime::MediaPreparer w;
	SuperEpicFuntime::MediaPreparerGUI w;
	w.show();

	return a.exec();
}
