#include "ui\mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MainWindow w;
	w.setGeometry(100, 100, 1200, 600);
	w.show();

	return a.exec();
}