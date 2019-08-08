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
    // Создаём сектора
    for (size_t i = 0; i < 19; ++i)
    {
        m_sectors.append(Sector{i, 64, 0, false});
    }

    m_sectors[0].size = 16;
    m_sectors[1].size = 8;
    m_sectors[2].size = 8;
    m_sectors[3].size = 32;

    // Заполняем адреса секторов
    size_t currentAddress = 0;
    for (auto &sector : m_sectors)
    {
        sector.address = currentAddress;
        currentAddress += sector.size;
    }
}


void SectorTableModel::setItemsSelected(const std::vector<int> &items, bool selected)
{
    for (const auto &item : items)
    {
        assert(item < m_sectors.size());

        m_sectors[item].selected = selected;
    }

    emit dataChanged(index(0, 0), index(m_sectors.size(), Column::LAST));
}


std::vector<SectorTableModel::Sector> SectorTableModel::getItems(const std::vector<int> &items) const
{
    std::vector<Sector> result;
    result.reserve(items.size());

    for (const auto &item : items)
    {
        result.emplace_back(m_sectors[item]);
    }

    return result;
}


std::vector<SectorTableModel::Sector> SectorTableModel::getItems() const
{
    std::vector<Sector> result;
    result.reserve(static_cast<size_t>(m_sectors.size()));

    for (const auto &sector : m_sectors)
    {
        result.emplace_back(sector);
    }

    return result;
}


std::vector<SectorTableModel::Sector> SectorTableModel::getSelectedItems() const
{
    std::vector<Sector> result;
    for (const auto &sector : m_sectors)
    {
        if (sector.selected)
        {
            result.emplace_back(sector);
        }
    }

    return result;
}


Qt::ItemFlags SectorTableModel::flags(const QModelIndex &index) const
{
    if (index.column() == Column::CHECKBOX)
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
    return Column::COUNT;
}


QVariant SectorTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal)
    {
        switch (section)
        {
            case Column::NAME:
                return QString("Сектор");

            case Column::SIZE:
                return QString("Размер");

            case Column::ADDRESS:
                return QString("Адреса");

            default:
                return {};
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
            case Column::NAME:
                return QString("SA%1").arg(index.row());

            case Column::SIZE:
                return QString("%1 KБ").arg(m_sectors[index.row()].size);

            case Column::ADDRESS:
            {
                const auto sector = m_sectors[index.row()];
                return QString("%1h-%2h")
                    .arg(QString::number(sector.address * 1024, 16), 6, '0')
                    .arg(QString::number((sector.address + sector.size) * 1024 - 1, 16), 6, '0');
            }

            default:
                break;
        }
    }

    if (role == Qt::CheckStateRole && index.column() == Column::CHECKBOX)
    {
        return m_sectors[index.row()].selected ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
    }

    return {};
}


bool SectorTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole && index.column() == Column::CHECKBOX)
    {
        m_sectors[index.row()].selected = value.value<bool>();

        emit dataChanged(index, index);
        return true;
    }

    return QAbstractItemModel::setData(index, value, role);
}

} // namespace app
