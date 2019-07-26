/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "MainWindow.h"

#include <QFileDialog>

namespace
{
constexpr auto WINDOW_WIDTH_MIN = 900;
constexpr auto WINDOW_HEIGHT_MIN = 600;
} // namespace


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
    connect(m_ui.getFileSelectionButton(), &QPushButton::clicked, [this]() {
        m_ui.getFileDialog()->show();
    });

    connect(m_ui.getFileDialog(), &QFileDialog::fileSelected, [this](const QString &file) {
        m_ui.setSelectedFilePath(file);
    });
}

} // namespace app
