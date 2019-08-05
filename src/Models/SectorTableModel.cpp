/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "SectorTableModel.h"

#include <cassert>

namespace app
{
SectorTableModel::SectorTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    for (int i = 0; i < 18; ++i)
    {
        m_sectors.append(Sector{64, false});
    }

    m_sectors[0].size = 16;
    m_sectors[1].size = 8;
    m_sectors[2].size = 8;
    m_sectors[3].size = 32;
}


void SectorTableModel::setItemsSelected(const std::vector<int> &items, bool selected)
{
    for (const auto &item : items)
    {
        assert(item < m_sectors.size());

        m_sectors[item].selected = selected;
    }

    emit dataChanged(index(0, 0), index(m_sectors.size(), 2));
}


std::vector<bool> SectorTableModel::getItemsState(const std::vector<int> &items) const
{
    std::vector<bool> result;
    result.reserve(items.size());

    for (const auto &item : items)
    {
        result.emplace_back(m_sectors[item].selected);
    }

    return result;
}


std::vector<bool> SectorTableModel::getItemsState() const
{
    std::vector<bool> result;
    result.reserve(static_cast<size_t>(m_sectors.size()));

    for (const auto &sector : m_sectors)
    {
        result.emplace_back(sector.selected);
    }

    return result;
}


Qt::ItemFlags SectorTableModel::flags(const QModelIndex &index) const
{
    if (index.column() == 2)
    {
        return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
    }

    return QAbstractTableModel::flags(index);
}


int SectorTableModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_sectors.size();
}


int SectorTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}


QVariant SectorTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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


QVariant SectorTableModel::data(const QModelIndex &index, int role) const
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
                return QString("%1 KБ").arg(m_sectors[index.row()].size);

            default:
                break;
        }
    }

    if (role == Qt::CheckStateRole && index.column() == 2)
    {
        return m_sectors[index.row()].selected ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
    }

    return {};
}


bool SectorTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole && index.column() == 2)
    {
        m_sectors[index.row()].selected = value.value<bool>();

        emit dataChanged(index, index);
        return true;
    }

    return QAbstractItemModel::setData(index, value, role);
}

} // namespace app
