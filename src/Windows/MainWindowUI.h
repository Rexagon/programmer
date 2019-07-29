#ifndef PROGRAMMER_MAINWINDOWUI_H
#define PROGRAMMER_MAINWINDOWUI_H

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>

#include "General.h"
#include "Widgets/ConnectionWidget.h"
#include "Widgets/LinkButton.h"
#include "Widgets/SectorPresetsWidget.h"

namespace app
{
class MainWindowUI final
{
public:
    explicit MainWindowUI(QMainWindow *window = nullptr);

    void setViewMode(ViewMode mode);

    void setConnectionState(ConnectionState state);

    void setApplicationState(ApplicationState state);

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

    ConnectionWidget *m_connectionWidget = nullptr;

    QStackedWidget *m_sectorSelectionWrapper = nullptr;
    SectorPresetsWidget *m_sectorPresets = nullptr;

    LinkButton *m_viewModeToggle = nullptr;
    QPushButton *m_verifyButton = nullptr;
    QPushButton *m_writeButton = nullptr;
};

} // namespace app

#endif // PROGRAMMER_MAINWINDOWUI_H
