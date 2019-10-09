/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "MainWindow.h"

#include <iostream>

#include <QMessageBox>

#include "OperationDialog.h"
#include "Operations/Dump.h"
#include "Operations/Program.h"
#include "Operations/Verify.h"
#include <Operations/Clear.h>

namespace
{
constexpr auto DEFAULT_CONNECTION_STATE = app::ConnectionState::DISCONNECTED;
constexpr auto DEFAULT_APPLICATION_STATE = app::ApplicationState::DISCONNECTED;

constexpr auto FILE_DIALOG_PATTERN = "(*.mem) ;; All files (*.*)";
} // namespace

namespace app
{
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , m_ui{m_sectorsTableModel, this}
    , m_connectionState{DEFAULT_CONNECTION_STATE}
    , m_applicationState{DEFAULT_APPLICATION_STATE}
{
    createFileDialog();

    syncState();

    connectSignals();
}


void MainWindow::connectProgrammer()
{
    m_connectionState = ConnectionState::CONNECTING;
    syncState();

    // Обновляем интерфейс перед потенциально долгой операцией
    repaint();

    try
    {
        const auto connectionWidget = m_ui.getConnectionWidget();

        const auto selectedPort = connectionWidget->getSelectedSerialPort();
        const auto selectedBaudRate = 115200;

        m_programmer = std::make_unique<Programmer>(selectedPort.portName().toStdString(), selectedBaudRate);

        connectionWidget->setInformationText(QString("Порт:\t\t\t%1\nВерсия программатора:\t%2\nВерсия SITL:\t\t%3")
                                                 .arg(selectedPort.portName())
                                                 .arg("unknown")
                                                 .arg("unknown"));

        m_connectionState = ConnectionState::CONNECTED;
        m_applicationState = ApplicationState::CONNECTED;
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Ошибка", "Невозможно подключиться к устройству\n");
        m_connectionState = ConnectionState::DISCONNECTED;
    }

    syncState();
}


void MainWindow::disconnectProgrammer()
{
    m_programmer.reset();

    m_connectionState = ConnectionState::DISCONNECTED;
    m_applicationState = ApplicationState::DISCONNECTED;
    syncState();
}


void MainWindow::runDumpOperation()
{
    selectFile([this](const QString &file) {
        runOperation(std::make_unique<Dump>(*m_programmer, m_sectorsTableModel, file));
    });
}


void MainWindow::runVerifyOperation()
{
    selectFile([this](const QString &file) {
        runOperation(std::make_unique<Verify>(*m_programmer, m_sectorsTableModel, file));
    });
}


void MainWindow::runWriteOperation()
{
    selectFile([this](const QString &file) {
        runOperation(std::make_unique<Program>(*m_programmer, m_sectorsTableModel, file));
    });
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


void MainWindow::selectFile(const std::function<void(const QString &)> &cb)
{
    m_fileDialog->disconnect();

    connect(m_fileDialog, &QFileDialog::fileSelected, [this, cb](const QString &file) {
        m_fileDialog->disconnect();
        cb(file);
    });

    m_fileDialog->open();
}


void MainWindow::createFileDialog()
{
    m_fileDialog = new QFileDialog(this);
    m_fileDialog->setFileMode(QFileDialog::AnyFile);
    m_fileDialog->setNameFilter(FILE_DIALOG_PATTERN);
}


void MainWindow::connectSignals()
{
    const auto connectionWidget = m_ui.getConnectionWidget();
    connect(connectionWidget, &ConnectionWidget::connectionRequest, this, &MainWindow::connectProgrammer);
    connect(connectionWidget, &ConnectionWidget::disconnectionRequest, this, &MainWindow::disconnectProgrammer);

    connect(m_ui.getDumpButton(), &QPushButton::clicked, this, &MainWindow::runDumpOperation);
    connect(m_ui.getVerifyButton(), &QPushButton::clicked, this, &MainWindow::runVerifyOperation);
    connect(m_ui.getWriteButton(), &QPushButton::clicked, this, &MainWindow::runWriteOperation);
}


void MainWindow::syncState()
{
    m_ui.setConnectionState(m_connectionState);
    m_ui.setApplicationState(m_applicationState);
}

} // namespace app
