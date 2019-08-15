#ifndef PROGRAMMER_MAINWINDOW_H
#define PROGRAMMER_MAINWINDOW_H

#include <memory>

#include <QFileDialog>

#include "MainWindowUI.h"
#include "Models/Programmer.h"
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
    void connectProgrammer();
    void disconnectProgrammer();

    void runVerifyOperation();
    void runWriteOperation();
    void runDumpOperation();
    void runClearOperation();

    void toggleViewMode();

    void runOperation(std::unique_ptr<Operation> operation);

    void selectFile(const std::function<void(const QString &)> &cb);

    void createFileDialog();
    void connectSignals();
    void syncState();

    MainWindowUI m_ui;

    ViewMode m_viewMode;
    ConnectionState m_connectionState;
    ApplicationState m_applicationState;

    std::unique_ptr<Programmer> m_programmer{};
    SectorTableModel m_sectorsTableModel{};

    QFileDialog *m_fileDialog = nullptr;
};

} // namespace app

#endif // PROGRAMMER_MAINWINDOW_H
