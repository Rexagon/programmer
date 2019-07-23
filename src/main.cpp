#include <iostream>

#include <QApplication>

#include "MainWindow.h"

int main(int argc, char **argv)
{
    QApplication programmer(argc, argv);

    app::MainWindow mainWindow;
    mainWindow.show();

    return programmer.exec();
}
