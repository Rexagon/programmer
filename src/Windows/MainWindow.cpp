/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "MainWindow.h"

#include <iostream>

#include <QMessageBox>

#include "OperationDialog.h"
#include "Operations/Program.h"
#include "Operations/Verify.h"
#include <Operations/Clear.h>

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
    createFileDialog();

    syncState();
    m_ui.setSectorsTableModel(&m_sectorsTableModel);

    connectSignals();
}


void MainWindow::connectSignals()
{
    connect(m_ui.getConnectionWidget(), &ConnectionWidget::connectionRequest, [this] {
        m_connectionState = ConnectionState::CONNECTING;
        syncState();

        // Обновляем интерфейс перед потенциально долгой операцией
        repaint();

        try
        {
            const auto selectedPort = m_ui.getConnectionWidget()->getSelectedSerialPort();
            const auto selectedBaudRate = m_ui.getConnectionWidget()->getSelectedBaudRate();

            std::cout << selectedPort.portName().toStdString() << std::endl;

            m_programmer = std::make_unique<Programmer>(selectedPort.portName().toStdString(), selectedBaudRate);

            m_connectionState = ConnectionState::CONNECTED;
            m_applicationState = ApplicationState::CONNECTED;
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(this, "Ошибка", "Невозможно подключиться к устройству\n");
            m_connectionState = ConnectionState::DISCONNECTED;
        }

        syncState();
    });

    connect(m_ui.getConnectionWidget(), &ConnectionWidget::disconnectionRequest, [this] {
        m_programmer.reset();

        m_connectionState = ConnectionState::DISCONNECTED;
        m_applicationState = ApplicationState::DISCONNECTED;
        syncState();
    });

    connect(m_ui.getViewModeToggle(), &LinkButton::clicked, [this] {
        m_viewMode = !m_viewMode;
        m_ui.setViewMode(m_viewMode);
    });

    connect(m_ui.getWriteButton(), &QPushButton::clicked, selectFileOnce([this](const QString &file) {
                runOperation(std::make_unique<Program>(&m_sectorsTableModel, file));
            }));

    connect(m_ui.getVerifyButton(), &QPushButton::clicked, selectFileOnce([this](const QString &file) {
                runOperation(std::make_unique<Verify>(&m_sectorsTableModel, file));
            }));

    connect(m_ui.getClearButton(), &QPushButton::clicked,
            [this]() { runOperation(std::make_unique<Clear>(&m_sectorsTableModel)); });
}


void MainWindow::syncState()
{
    m_ui.setViewMode(m_viewMode);
    m_ui.setConnectionState(m_connectionState);
    m_ui.setApplicationState(m_applicationState);
}


void MainWindow::createFileDialog()
{
    m_fileDialog = new QFileDialog(this);
    m_fileDialog->setFileMode(QFileDialog::AnyFile);
    m_fileDialog->setNameFilter(FILE_DIALOG_PATTERN);
}


void MainWindow::runOperation(std::unique_ptr<Operation> operation)
{
    const auto error = operation->validate();

    if (error)
    {
        QMessageBox::critical(this, "Ошибка", *error);
        activateWindow();
        return;
    }

    auto *dialog = new OperationDialog(std::move(operation), this);
    dialog->open();
}


std::function<void()> MainWindow::selectFileOnce(const std::function<void(const QString &)> &cb)
{
    return [this, cb] {
        connect(m_fileDialog, &QFileDialog::fileSelected, [this, cb](const QString &file) {
            m_fileDialog->disconnect();
            cb(file);
        });

        m_fileDialog->open();
    };
}

} // namespace app
