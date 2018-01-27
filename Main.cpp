#include "DAO.h"
#include "MainWindow.h"
#include <QApplication>

// workaround for a bug on Mavericks
// Finder returns / as the working path of an app bundle
// but if the app is run from terminal, the path is correct
// This method calcluates the path of the bundle from the application's path
QString getCurrentPath()
{
    QDir dir(QApplication::applicationDirPath());
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    return dir.absolutePath();
}

QString dbFileName;

int main(int argc, char *argv[])
{
    dbFileName = "MedicalExpenses.db";

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

#ifdef Q_OS_OSX
    QDir::setCurrent(getCurrentPath());
#endif

    DAO::getInstance();
    MainWindow mainWindow;
    mainWindow.showMaximized();
    return app.exec();
}
