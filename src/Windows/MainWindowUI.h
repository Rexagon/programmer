#ifndef PROGRAMMER_MAINWINDOWUI_H
#define PROGRAMMER_MAINWINDOWUI_H

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>

#include "General.h"
#include "Models/SectorPresetsModel.h"
#include "Widgets/ConnectionWidget.h"
#include "Widgets/SectorPresetsWidget.h"

namespace app
{
class MainWindowUI final
{
public:
    /**
     * @param model     Пресеты секторов
     * @param window    Родительское окно
     */
    explicit MainWindowUI(SectorPresetsModel &model, QMainWindow *window);

    /**
     * @brief       Устанавливает набор отображаемых элементов
     *              в зависимости от состояния подключения
     *
     * @param state Состояние подключения
     */
    void setConnectionState(ConnectionState state);

    /**
     * @brief       Устанавливает набор отображаемых элементов
     *              в зависимости от состояния приложения
     *
     * @param state Состояние приложения
     */
    void setApplicationState(ApplicationState state);

    /**
     * @return  Виджет подключения к программатору
     */
    ConnectionWidget *getConnectionWidget() const;

    /**
     * @return  Кнопка операции считывания
     */
    QPushButton *getDumpButton() const;

    /**
     * @return  Кнопка операции проверки
     */
    QPushButton *getVerifyButton() const;

    /**
     * @return  Кнопка операции записи
     */
    QPushButton *getWriteButton() const;

public:
    MainWindowUI(const MainWindowUI &) = delete;
    MainWindowUI &operator=(const MainWindowUI &) = delete;
    MainWindowUI(MainWindowUI &&) noexcept = delete;
    MainWindowUI &operator=(MainWindowUI &&) noexcept = delete;

private:
    QWidget *createTopWorkspace(SectorPresetsModel &model);
    QWidget *createBottomWorkspace();

    QMainWindow *m_window;

    ConnectionWidget *m_connectionWidget = nullptr;

    SectorPresetsWidget *m_sectorPresetsWidget = nullptr;

    QPushButton *m_dumpButton = nullptr;
    QPushButton *m_verifyButton = nullptr;
    QPushButton *m_writeButton = nullptr;
};

} // namespace app

#endif // PROGRAMMER_MAINWINDOWUI_H
