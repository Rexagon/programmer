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
    m_baudRateLabel->setVisible(mode == ViewMode::EXTENDED);
    m_baudRateSelector->setVisible(mode == ViewMode::EXTENDED);
}


void ConnectionWidget::setConnectionState(ConnectionState state)
{
    m_baudRateSelector->setEnabled(state != ConnectionState::CONNECTING);
    m_serialPortSelector->setEnabled(state != ConnectionState::CONNECTING);
    m_connectionToggleButton->setEnabled(state != ConnectionState::CONNECTING);

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

    m_state = state;
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
    auto *layout = new QVBoxLayout(this);

    // Serial port selector
    auto *serialPortLabel = new QLabel("Порт:", this);
    layout->addWidget(serialPortLabel);

    m_serialPortSelector = new SerialPortSelector(this);
    layout->addWidget(m_serialPortSelector);

    // Baud rate selector
    m_baudRateLabel = new QLabel("Baud:", this);
    layout->addWidget(m_baudRateLabel);

    m_baudRateSelector = new BaudRateSelector(this);
    layout->addWidget(m_baudRateSelector);

    // Space
    layout->addSpacing(11);

    // Connection button
    m_connectionToggleButton = new QPushButton(this);
    layout->addWidget(m_connectionToggleButton);
}


void ConnectionWidget::connectSignals()
{
    auto OnChanged = static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged);

    connect(m_serialPortSelector, OnChanged, [this](int index) {
        emit serialPortChanged(m_serialPortSelector->itemData(index).value<QSerialPortInfo>());
    });

    connect(m_baudRateSelector, OnChanged,
            [this](int index) { emit baudRateChanged(m_baudRateSelector->itemData(index).value<qint32>()); });

    connect(m_connectionToggleButton, &QPushButton::clicked, [this]() {
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
