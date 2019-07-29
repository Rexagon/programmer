/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "BaudRateSelector.h"

#include <QtSerialPort/QSerialPortInfo>

namespace
{
constexpr auto DEFAULT_BAUD_RATE = 115200;
}


namespace app
{
BaudRateSelector::BaudRateSelector(QWidget *parent)
    : QComboBox{parent}
{
    const auto baudRates = QSerialPortInfo::standardBaudRates();

    for (const auto &rate : baudRates)
    {
        addItem(QString::number(rate), rate);
    }

    const auto index = findData(static_cast<qint32>(DEFAULT_BAUD_RATE));
    if (index > 0)
    {
        setCurrentIndex(index);
    }
}

} // namespace app
