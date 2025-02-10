#include "HexagonGenerator.h"
#include <QtWidgets/QApplication>
#include <ResourceManager.h>
#include <QSurfaceFormat>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ResourceManager::initializePixmaps();

	QSurfaceFormat format;
	format.setSamples(4);
	QSurfaceFormat::setDefaultFormat(format);

	HexagonGenerator w;
	w.show();
	return a.exec();
}
