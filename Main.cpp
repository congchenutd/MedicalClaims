#include "DAO.h"
#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	DAO::getInstance();
    MainWindow mainWindow;
	mainWindow.resize(1024, 600);
	mainWindow.show();
    return app.exec();
}
