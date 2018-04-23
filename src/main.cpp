#include <MediaPreparerGUI.hpp>
#include <QtWidgets/QApplication>

// class SuperEpicFuntime::Event;

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	// SuperEpicFuntime::MediaPreparer w;
	SuperEpicFuntime::MediaPreparerGUI w;

	w.show();

	return a.exec();
}
