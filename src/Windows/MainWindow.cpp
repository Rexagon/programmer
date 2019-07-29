/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "MainWindow.h"

#include <QFileDialog>

namespace
{
constexpr auto DEFAULT_VIEW_MODE = app::ViewMode::COMPACT;
constexpr auto DEFAULT_CONNECTION_STATE = app::ConnectionState::CONNECTED;
constexpr auto DEFAULT_APPLICATION_STATE = app::ApplicationState::CONNECTED;
} // namespace

namespace app
{
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , m_ui{this}
    , m_viewMode{DEFAULT_VIEW_MODE}
    , m_connectionState{DEFAULT_CONNECTION_STATE}
    , m_applicationState{DEFAULT_APPLICATION_STATE}
{
    m_ui.setViewMode(m_viewMode);
    m_ui.setConnectionState(m_connectionState);
    m_ui.setApplicationState(m_applicationState);
    m_ui.setSectorsTableModel(&m_sectorsTableModel);

    connectSignals();
}


void MainWindow::connectSignals()
{
    connect(m_ui.getViewModeToggle(), &LinkButton::clicked, [this]() {
        m_viewMode = !m_viewMode;
        m_ui.setViewMode(m_viewMode);
    });

    connect(m_ui.getWriteButton(), &QPushButton::clicked, [this]() {
        QFileDialog::getOpenFileName(this, "Open Document", QDir::currentPath(), "(*.mem) ;; All files (*.*)");
    });
}

} // namespace app
