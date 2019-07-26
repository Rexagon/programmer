/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "SerialPortListModel.h"

namespace app
{
app::SerialPortListModel::SerialPortListModel(QObject *parent)
    : QAbstractListModel{parent}
{
}


void SerialPortListModel::refresh()
{
    m_serialPorts = QSerialPortInfo::availablePorts();
}


int SerialPortListModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_serialPorts.size();
}


QVariant SerialPortListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    if (role == Qt::DisplayRole)
    {
        return m_serialPorts[index.row()].portName();
    }

    return {};
}

} // namespace app
