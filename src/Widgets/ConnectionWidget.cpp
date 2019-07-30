/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "ConnectionWidget.h"

#include <QVBoxLayout>

namespace app
{
ConnectionWidget::ConnectionWidget(QWidget *parent)
    : QWidget{parent}
{
    createUI();
    connectSignals();
}


void ConnectionWidget::setViewMode(ViewMode mode)
{
    m_viewMode = mode;

    m_baudRateLabel->setVisible(mode == ViewMode::EXTENDED && m_state != ConnectionState::CONNECTED);
    m_baudRateSelector->setVisible(mode == ViewMode::EXTENDED && m_state != ConnectionState::CONNECTED);
}


void ConnectionWidget::setConnectionState(ConnectionState state)
{
    m_state = state;

    m_baudRateSelector->setEnabled(state == ConnectionState::DISCONNECTED);
    m_serialPortSelector->setEnabled(state == ConnectionState::DISCONNECTED);
    m_connectionToggleButton->setEnabled(state != ConnectionState::CONNECTING);

    m_serialPortLabel->setVisible(state != ConnectionState::CONNECTED);
    m_serialPortSelector->setVisible(state != ConnectionState::CONNECTED);
    m_baudRateLabel->setVisible(state != ConnectionState::CONNECTED && m_viewMode == ViewMode::EXTENDED);
    m_baudRateSelector->setVisible(state != ConnectionState::CONNECTED && m_viewMode == ViewMode::EXTENDED);

    m_connectionInfoLabel->setVisible(state == ConnectionState::CONNECTED);

    QString buttonText{};
    switch (state)
    {
        case ConnectionState::DISCONNECTED:
            buttonText = "Подключить";
            break;

        case ConnectionState::CONNECTING:
            buttonText = "Подключение...";
            break;

        case ConnectionState::CONNECTED:
            buttonText = "Отключить";
            break;
    }

    m_connectionToggleButton->setText(buttonText);
}


QSerialPortInfo ConnectionWidget::getSelectedSerialPort() const
{
    return m_serialPortSelector->itemData(m_serialPortSelector->currentIndex()).value<QSerialPortInfo>();
}


qint32 ConnectionWidget::getSelectedBaudRate() const
{
    return m_baudRateSelector->itemData(m_baudRateSelector->currentIndex()).value<qint32>();
}


SerialPortSelector *ConnectionWidget::getSerialPortSelector() const
{
    return m_serialPortSelector;
}


BaudRateSelector *ConnectionWidget::getBaudRateSelector() const
{
    return m_baudRateSelector;
}


QPushButton *ConnectionWidget::getConnectionToggleButton() const
{
    return m_connectionToggleButton;
}


void ConnectionWidget::createUI()
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    auto *layout = new QVBoxLayout(this);

    // Serial port selector
    m_serialPortLabel = new QLabel("Порт:", this);
    layout->addWidget(m_serialPortLabel);

    m_serialPortSelector = new SerialPortSelector(this);
    layout->addWidget(m_serialPortSelector);

    // Serial port selector
    m_baudRateLabel = new QLabel("Baud:", this);
    layout->addWidget(m_baudRateLabel);

    m_baudRateSelector = new BaudRateSelector(this);
    layout->addWidget(m_baudRateSelector);

    // Info page
    m_connectionInfoLabel = new QLabel("Порт:\t\t\tCOM1\nВерсия программатора:\t1.0\nВерсия SITL:\t\t1.0", this);
    layout->addWidget(m_connectionInfoLabel);

    // Space
    layout->addSpacing(11);

    // Connection button
    m_connectionToggleButton = new QPushButton(this);
    layout->addWidget(m_connectionToggleButton);
}


void ConnectionWidget::connectSignals()
{
    auto OnChanged = static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged);

    connect(m_serialPortSelector, OnChanged, [this](int index)
    {
        emit serialPortChanged(m_serialPortSelector->itemData(index).value<QSerialPortInfo>());
    });

    connect(m_baudRateSelector, OnChanged,
            [this](int index) { emit baudRateChanged(m_baudRateSelector->itemData(index).value<qint32>()); });

    connect(m_connectionToggleButton, &QPushButton::clicked, [this]()
    {
        switch (m_state)
        {
            case ConnectionState::DISCONNECTED:
                emit connectionRequest();
                break;

            case ConnectionState::CONNECTED:
                emit disconnectionRequest();

            default:
                break;
        }
    });
}

} // namespace app
