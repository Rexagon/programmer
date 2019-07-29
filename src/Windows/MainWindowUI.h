#ifndef PROGRAMMER_MAINWINDOWUI_H
#define PROGRAMMER_MAINWINDOWUI_H

#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>

#include "Widgets/ConnectionWidget.h"
#include "Widgets/LinkButton.h"

namespace app
{
class MainWindowUI final
{
public:
    enum ViewMode
    {
        COMPACT,
        EXTENDED
    };

    explicit MainWindowUI(QMainWindow *window = nullptr);

    void toggleExtended();
    void setExtended(bool extended);
    bool isExtended() const;

    ConnectionWidget *getConnectionWidget() const;

    LinkButton *getViewModeToggle() const;

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

    bool m_isExtended;

    ConnectionWidget *m_connectionWidget = nullptr;

    LinkButton *m_viewModeToggle = nullptr;
    QPushButton *m_verifyButton = nullptr;
    QPushButton *m_writeButton = nullptr;
};

} // namespace app

#endif // PROGRAMMER_MAINWINDOWUI_H
