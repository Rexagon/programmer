#ifndef PROGRAMMER_MAINWINDOW_H
#define PROGRAMMER_MAINWINDOW_H

#include "MainWindowUI.h"

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
};

} // namespace app

#endif // PROGRAMMER_MAINWINDOW_H
