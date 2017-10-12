#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("PController");
	app.setOrganizationName("Spankjet");

	MainWindow w;
	w.show();
	return app.exec();
}
