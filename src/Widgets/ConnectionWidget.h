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
class ConnectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionWidget(QWidget *parent = nullptr);

    void setViewMode(ViewMode mode);

    void setConnectionState(ConnectionState state);

    QSerialPortInfo getSelectedSerialPort() const;
    qint32 getSelectedBaudRate() const;

    SerialPortSelector *getSerialPortSelector() const;
    BaudRateSelector *getBaudRateSelector() const;
    QPushButton *getConnectionToggleButton() const;

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
