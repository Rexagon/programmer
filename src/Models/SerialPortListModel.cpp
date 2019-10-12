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


bool SerialPortListModel::isEmpty() const
{
    return m_serialPorts.empty();
}


int SerialPortListModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_serialPorts.size();
}


const QList<QSerialPortInfo>& SerialPortListModel::getSerialPorts() const
{
    return m_serialPorts;
}


QVariant SerialPortListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    if (role == Qt::UserRole)
    {
        return QVariant::fromValue(m_serialPorts[index.row()]);
    }

    if (role == Qt::DisplayRole)
    {
        auto description = m_serialPorts[index.row()].description();
        auto name = m_serialPorts[index.row()].portName();

        return QString("%1 (%2)").arg(description, name);
    }

    return {};
}

} // namespace app
