#include <Controller.hpp>
#include <MediaPreparer.hpp>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	SuperEpicFuntime::Controller c(new SuperEpicFuntime::MediaPreparer());

	c.show();

	return app.exec();
}
