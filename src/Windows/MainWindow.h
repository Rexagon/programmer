#ifndef PROGRAMMER_MAINWINDOW_H
#define PROGRAMMER_MAINWINDOW_H

#include <memory>

#include <QFileDialog>

#include "MainWindowUI.h"
#include "Models/SectorTableModel.h"
#include "Operations/Operation.h"

namespace app
{
class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

    MainWindow(const MainWindow &) = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow(MainWindow &&) noexcept = delete;
    MainWindow &operator=(MainWindow &&) noexcept = delete;

private:
    void createFileDialog();
    void connectSignals();

    void syncState();

    void runOperation(std::unique_ptr<Operation> operation);

    MainWindowUI m_ui;

    ViewMode m_viewMode;
    ConnectionState m_connectionState;
    ApplicationState m_applicationState;

    SectorTableModel m_sectorsTableModel{};

    QFileDialog *m_fileDialog = nullptr;
};

} // namespace app

#endif // PROGRAMMER_MAINWINDOW_H
