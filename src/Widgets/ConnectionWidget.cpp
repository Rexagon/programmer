/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "ConnectionWidget.h"

#include <QVBoxLayout>

namespace
{
constexpr auto DEFAULT_VIEW_MODE = app::ConnectionWidget::ViewMode::COMPACT;
constexpr auto DEFAULT_STATE = app::ConnectionWidget::State::DISCONNECTED;
} // namespace

namespace app
{
ConnectionWidget::ConnectionWidget(QWidget *parent)
    : QWidget{parent}
    , m_viewMode{DEFAULT_VIEW_MODE}
    , m_state{DEFAULT_STATE}
{
    createUI();
    setViewMode(m_viewMode);
    setState(m_state);

    connectSignals();
}


void ConnectionWidget::setViewMode(app::ConnectionWidget::ViewMode mode)
{
    m_baudRateLabel->setVisible(mode == ViewMode::EXTENDED);
    m_baudRateSelector->setVisible(mode == ViewMode::EXTENDED);

    m_viewMode = mode;
}


ConnectionWidget::ViewMode ConnectionWidget::getViewMode() const
{
    return m_viewMode;
}


void ConnectionWidget::setState(State state)
{
    m_baudRateSelector->setEnabled(state != State::CONNECTING);
    m_serialPortSelector->setEnabled(state != State::CONNECTING);
    m_connectionToggleButton->setEnabled(state != State::CONNECTING);

    QString buttonText{};
    switch (state)
    {
        case State::DISCONNECTED:
            buttonText = "Подключить";
            break;

        case State::CONNECTING:
            buttonText = "Подключение...";
            break;

        case State::CONNECTED:
            buttonText = "Отключить";
            break;
    }

    m_connectionToggleButton->setText(buttonText);

    m_state = state;
}


ConnectionWidget::State ConnectionWidget::getState() const
{
    return m_state;
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
            case State::DISCONNECTED:
                emit connectionRequest();
                break;

            case State::CONNECTED:
                emit disconnectionRequest();

            default:
                break;
        }
    });
}

} // namespace app
