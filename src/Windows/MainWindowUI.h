#ifndef PROGRAMMER_MAINWINDOWUI_H
#define PROGRAMMER_MAINWINDOWUI_H

#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>

#include "Widgets/SerialPortSelector.h"

namespace app
{
class MainWindowUI final
{
public:
    explicit MainWindowUI(QMainWindow *window = nullptr);

    SerialPortSelector *getSerialPortSelector() const;

    QFileDialog *getFileDialog() const;
    QPushButton *getFileSelectionButton() const;
    void setSelectedFilePath(const QString &path);

    QProgressBar *getProgressBar() const;
    QPushButton *getVerifyButton() const;
    QPushButton *getWriteButton() const;

    MainWindowUI(const MainWindowUI &) = delete;
    MainWindowUI &operator=(const MainWindowUI &) = delete;
    MainWindowUI(MainWindowUI &&) noexcept = delete;
    MainWindowUI &operator=(MainWindowUI &&) noexcept = delete;

private:
    QWidget *createTopWorkspace();
    QWidget *createBottomWorkspace();

    QMainWindow *m_window;

    SerialPortSelector *m_serialPortSelector = nullptr;

    QFileDialog *m_fileDialog = nullptr;
    QPushButton *m_fileSelectionButton = nullptr;
    QLabel *m_selectedFileLabel = nullptr;

    QProgressBar *m_progressBar = nullptr;
    QPushButton *m_verifyButton = nullptr;
    QPushButton *m_writeButton = nullptr;
};

} // namespace app

#endif // PROGRAMMER_MAINWINDOWUI_H
