#include <QtWidgets/QApplication>
#include <src/MediaPreparer.hpp>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	//	QCoreApplication::setOrganizationName("SuperEpicFuntime");
	//	QCoreApplication::setOrganizationDomain("https://superepicfuntime.com");
	//	QCoreApplication::setApplicationName("SEFMediaPreparer");

	SuperEpicFuntime::MediaPreparer w;
	w.show();

	return a.exec();
}
