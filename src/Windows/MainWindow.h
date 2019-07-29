#ifndef PROGRAMMER_MAINWINDOW_H
#define PROGRAMMER_MAINWINDOW_H

#include "MainWindowUI.h"
#include "Models/SectorsTableModel.h"

namespace app
{
class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private:
    void connectSignals();

    MainWindowUI m_ui;

    ViewMode m_viewMode;
    ConnectionState m_connectionState;
    ApplicationState m_applicationState;
    
    SectorsTableModel m_sectorsTableModel{};
};

} // namespace app

#endif // PROGRAMMER_MAINWINDOW_H
