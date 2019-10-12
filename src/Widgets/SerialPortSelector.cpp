/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "SerialPortSelector.h"

#include "../Settings.h"

namespace app
{
SerialPortSelector::SerialPortSelector(QWidget *parent)
    : QComboBox{parent}
    , m_serialPortListModel{parent}
{
    setModel(&m_serialPortListModel);
    m_serialPortListModel.refresh();

    if (m_serialPortListModel.isEmpty())
    {
        return;
    }

    //
    const auto &ports = m_serialPortListModel.getSerialPorts();
    const auto portName = Settings::getInstance().loadSelectedSerialPort();

    int index = 0;
    for (int i = 0; i < ports.size(); ++i)
    {
        if (ports[i].portName() == portName)
        {
            index = i;
            break;
        }
    }

    setCurrentIndex(index);

    //
    connectSignals();
}


void SerialPortSelector::showPopup()
{
    m_serialPortListModel.refresh();

    QComboBox::showPopup();
}


void SerialPortSelector::connectSignals()
{
    const auto OnChanged = static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged);

    connect(this, OnChanged, [this](int index) {
        const auto portInfo = itemData(index).value<QSerialPortInfo>();
        Settings::getInstance().saveSelectedSerialPort(portInfo.portName());
    });
}

} // namespace app
