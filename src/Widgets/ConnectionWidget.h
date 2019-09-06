#ifndef PROGRAMMER_CONNECTIONWIDGET_H
#define PROGRAMMER_CONNECTIONWIDGET_H

#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

#include "BaudRateSelector.h"
#include "General.h"
#include "SerialPortSelector.h"

namespace app
{
/**
 * @brief   Виджет подключения к программатору
 */
class ConnectionWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @param parent    Родительский виджет
     */
    explicit ConnectionWidget(QWidget *parent);

    /**
     * @brief       Устанавливает набор отображаемых полей в
     *              зависимости от режима отображения
     *
     * При @a ViewMode::Compact скрывается поля связанные
     * с выбором скорости подключения.
     *
     * @param mode  Режим отображения
     */
    void setViewMode(ViewMode mode);

    /**
     * @brief       Устанавливает набор отображаемых полей в
     *              зависимости от состояния подключения
     *
     * @param state Состояние подключения
     */
    void setConnectionState(ConnectionState state);

    /**
     * @brief       Устанавливает текст с описанием подключённого
     *              программатора
     *
     * Текст показывается только при состоянии подключения
     * @a ConnectionState::CONNECTED
     *
     * @param text  Описание подключённого программатора
     */
    void setInformationText(const QString &text);

    /**
     * @return  Выбранный COM порт
     */
    QSerialPortInfo getSelectedSerialPort() const;

    /**
     * @return  Ввыбранная скорость подключения
     */
    qint32 getSelectedBaudRate() const;

    /**
     * @return  Виджет выбора COM порта
     */
    SerialPortSelector *getSerialPortSelector() const;

    /**
     * @return  Виджет выбора скорости подключения
     */
    BaudRateSelector *getBaudRateSelector() const;

    /**
     * @return  Кнопка подключения
     */
    QPushButton *getConnectionToggleButton() const;

public:
    ConnectionWidget(const ConnectionWidget &) = delete;
    ConnectionWidget &operator=(const ConnectionWidget &) = delete;
    ConnectionWidget(ConnectionWidget &&) noexcept = delete;
    ConnectionWidget &operator=(ConnectionWidget &&) noexcept = delete;

signals:
    void serialPortChanged(const QSerialPortInfo &portInfo);
    void baudRateChanged(qint32 baudRate);
    void connectionRequest();
    void disconnectionRequest();

private:
    void createUI();
    void connectSignals();

    ViewMode m_viewMode{};
    ConnectionState m_state{};

    QLabel *m_serialPortLabel = nullptr;
    SerialPortSelector *m_serialPortSelector = nullptr;
    QLabel *m_baudRateLabel = nullptr;
    BaudRateSelector *m_baudRateSelector = nullptr;

    QLabel *m_connectionInfoLabel = nullptr;

    QPushButton *m_connectionToggleButton = nullptr;
};

} // namespace app

#endif // PROGRAMMER_CONNECTIONWIDGET_H
