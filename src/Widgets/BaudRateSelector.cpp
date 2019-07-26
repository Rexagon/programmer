/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "BaudRateSelector.h"

#include <QtSerialPort/QSerialPortInfo>

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
}

} // namespace app
