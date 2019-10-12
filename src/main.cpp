#include <QApplication>

#include "Windows/MainWindow.h"

int main(int argc, char **argv)
{
    QApplication programmer(argc, argv);
    QApplication::setOrganizationName("RC MODULE");
    QApplication::setApplicationName("Programmer");
    QApplication::setApplicationVersion(APP_VERSION);

    app::MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
