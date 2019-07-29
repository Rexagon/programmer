/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "MainWindow.h"

#include <QFileDialog>

namespace app
{
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , m_ui{this}
{
    connectSignals();
}


void MainWindow::connectSignals()
{
    connect(m_ui.getViewModeToggle(), &LinkButton::clicked, [this]() { m_ui.toggleExtended(); });

    connect(m_ui.getWriteButton(), &QPushButton::clicked, [this]() {
        QFileDialog::getOpenFileName(this, "Open Document", QDir::currentPath(), "(*.mem) ;; All files (*.*)");
    });
}

} // namespace app
