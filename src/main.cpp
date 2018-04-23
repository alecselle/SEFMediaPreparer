#include <MediaPreparerController.hpp>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	SuperEpicFuntime::MediaPreparerController c;

	c.show();

	return a.exec();
}
