/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "SectorsTableModel.h"

namespace app
{
SectorsTableModel::SectorsTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    for (int i = 0; i < 18; ++i)
    {
        m_sectors.append(Sector{64, false});
    }

    m_sectors[0].size = 16;
    m_sectors[1].size = 8;
    m_sectors[2].size = 8;
    m_sectors[4].size = 32;
}


void SectorsTableModel::selectAll()
{
    for (auto &sector : m_sectors)
    {
        sector.selected = true;
    }

    emit dataChanged(index(0, 0), index(m_sectors.size(), 2));
}


void SectorsTableModel::deselectAll()
{
    for (auto &sector : m_sectors)
    {
        sector.selected = true;
    }

    emit dataChanged(index(0, 0), index(m_sectors.size(), 2));
}


void SectorsTableModel::setItemsSelected(const std::vector<int> &items, bool selected)
{
    for (const auto &item : items)
    {
        if (item < m_sectors.size())
        {
            m_sectors[item].selected = selected;
        }
    }

    emit dataChanged(index(0, 0), index(m_sectors.size(), 2));
}


Qt::ItemFlags SectorsTableModel::flags(const QModelIndex &index) const
{
    if (index.column() == 2)
    {
        return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
    }

    return QAbstractTableModel::flags(index);
}


int SectorsTableModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_sectors.size();
}


int SectorsTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}


QVariant SectorsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 0:
                return QString("SA%1").arg(index.row());

            case 1:
                return QString("%1KБ").arg(m_sectors[index.row()].size);

            default:
                break;
        }
    }

    if (index.column() == 2 && role == Qt::CheckStateRole)
    {
        return m_sectors[index.row()].selected ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
    }


    return {};
}


bool SectorsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == 2 && role == Qt::CheckStateRole)
    {
        m_sectors[index.row()].selected = value.value<bool>();
        return true;
    }

    return QAbstractItemModel::setData(index, value, role);
}


QVariant SectorsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal)
    {
        switch (section)
        {
            case 0:
                return QString("Сектор");

            case 1:
                return QString("Размер");

            case 2:
                return {};

            default:
                break;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

} // namespace app
