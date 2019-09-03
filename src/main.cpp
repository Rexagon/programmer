#include <iostream>

#include <QApplication>

#include "Windows/MainWindow.h"

int main(int argc, char **argv)
{
    QApplication programmer(argc, argv);

    app::MainWindow mainWindow;
    mainWindow.show();

    return programmer.exec();
}
