#include <QtWidgets/QApplication>
#include <src/MediaPreparer.hpp>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MediaPreparer w;
	w.show();

	return a.exec();
}
