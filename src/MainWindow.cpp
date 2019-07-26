/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "MainWindow.h"

#include <QLabel>

namespace app
{
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    new QLabel("Hello world!", this);
}

} // namespace app
