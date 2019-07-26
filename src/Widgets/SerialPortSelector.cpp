/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "SerialPortSelector.h"

#include <QAbstractListModel>

#include <QtSerialPort/QSerialPortInfo>

namespace app
{
SerialPortSelector::SerialPortSelector(QWidget *parent)
    : QComboBox{parent}
    , m_serialPortListModel{parent}
{
    setModel(&m_serialPortListModel);
}


void SerialPortSelector::showPopup()
{
    m_serialPortListModel.refresh();

    QComboBox::showPopup();
}

} // namespace app
