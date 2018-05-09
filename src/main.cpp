#include <Controller.hpp>
#include <MediaPreparer.hpp>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	SuperEpicFuntime::MediaPreparer::Controller c = SuperEpicFuntime::MediaPreparer::Controller();

	c.show();

	return app.exec();
}
