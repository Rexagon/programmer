/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "MainWindow.h"

#include <QFileDialog>

namespace
{
constexpr auto DEFAULT_VIEW_MODE = app::ViewMode::COMPACT;
constexpr auto DEFAULT_CONNECTION_STATE = app::ConnectionState::DISCONNECTED;
constexpr auto DEFAULT_APPLICATION_STATE = app::ApplicationState::DISCONNECTED;

constexpr auto FILE_DIALOG_PATTERN = "(*.mem) ;; All files (*.*)";
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
    syncState();
    m_ui.setSectorsTableModel(&m_sectorsTableModel);

    connectSignals();
}


void MainWindow::connectSignals()
{
    connect(m_ui.getConnectionWidget(), &ConnectionWidget::connectionRequest, [this]() {
        m_connectionState = ConnectionState::CONNECTING;
        syncState();

        // TODO: connect via sitl

        m_connectionState = ConnectionState::CONNECTED;
        m_applicationState = ApplicationState::CONNECTED;
        syncState();
    });

    connect(m_ui.getConnectionWidget(), &ConnectionWidget::disconnectionRequest, [this]() {
        m_connectionState = ConnectionState::DISCONNECTED;
        m_applicationState = ApplicationState::DISCONNECTED;
        syncState();
    });

    connect(m_ui.getViewModeToggle(), &LinkButton::clicked, [this]() {
        m_viewMode = !m_viewMode;
        m_ui.setViewMode(m_viewMode);
    });

    connect(m_ui.getWriteButton(), &QPushButton::clicked, [this]() {
        const auto memoryFile =
            QFileDialog::getOpenFileName(this, "Выберите файл прошивки", QDir::currentPath(), FILE_DIALOG_PATTERN);
    });
}


void MainWindow::syncState()
{
    m_ui.setViewMode(m_viewMode);
    m_ui.setConnectionState(m_connectionState);
    m_ui.setApplicationState(m_applicationState);
}

} // namespace app
