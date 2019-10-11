#ifndef PROGRAMMER_MAINWINDOW_H
#define PROGRAMMER_MAINWINDOW_H

#include <memory>

#include <QFileDialog>

#include "MainWindowUI.h"
#include "Models/Programmer.h"
#include "Models/SectorPresetsModel.h"
#include "OperationDialog.h"
#include "Operations/Operation.h"

namespace app
{
class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

public:
    MainWindow(const MainWindow &) = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow(MainWindow &&) noexcept = delete;
    MainWindow &operator=(MainWindow &&) noexcept = delete;

private:
    void connectProgrammer();
    void disconnectProgrammer();

    void runDumpOperation();
    void runVerifyOperation();
    void runWriteOperation();

    void runOperation(std::unique_ptr<Operation> operation);

    void selectFile(const std::function<void(const QString &)> &cb);

    void createFileDialog();
    void connectSignals();
    void syncState();

    void closeEvent(QCloseEvent *event) override;

    SectorPresetsModel m_sectorPresetsModel{};
    std::unique_ptr<Programmer> m_programmer = nullptr;

    MainWindowUI m_ui;

    ConnectionState m_connectionState;
    ApplicationState m_applicationState;

    QFileDialog *m_fileDialog = nullptr;
    std::unique_ptr<OperationDialog> m_operationDialog = nullptr;
};

} // namespace app

#endif // PROGRAMMER_MAINWINDOW_H
