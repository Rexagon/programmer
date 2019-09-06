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
#include "Widgets/SectorTableWidget.h"

namespace app
{
class MainWindowUI final
{
public:
    /**
     * @param model     Таблица секторов
     * @param window    Родительское окно
     */
    explicit MainWindowUI(SectorTableModel &model, QMainWindow *window);

    void setViewMode(ViewMode mode);

    void setConnectionState(ConnectionState state);

    void setApplicationState(ApplicationState state);

    /**
     * @return  Виджет подключения к программатору
     */
    ConnectionWidget *getConnectionWidget() const;

    LinkButton *getViewModeToggle() const;

    QPushButton *getDumpButton() const;
    QPushButton *getClearButton() const;

    QPushButton *getVerifyButton() const;
    QPushButton *getWriteButton() const;

public:
    MainWindowUI(const MainWindowUI &) = delete;
    MainWindowUI &operator=(const MainWindowUI &) = delete;
    MainWindowUI(MainWindowUI &&) noexcept = delete;
    MainWindowUI &operator=(MainWindowUI &&) noexcept = delete;

private:
    QWidget *createTopWorkspace(SectorTableModel &model);
    QWidget *createBottomWorkspace();

    QMainWindow *m_window;

    ConnectionWidget *m_connectionWidget = nullptr;

    QStackedWidget *m_sectorsSelectionWrapper = nullptr;
    SectorPresetsWidget *m_sectorPresets = nullptr;
    SectorTableWidget *m_sectorTable = nullptr;

    LinkButton *m_viewModeToggle = nullptr;

    QPushButton *m_dumpButton = nullptr;
    QPushButton *m_clearButton = nullptr;

    QPushButton *m_verifyButton = nullptr;
    QPushButton *m_writeButton = nullptr;
};

} // namespace app

#endif // PROGRAMMER_MAINWINDOWUI_H
