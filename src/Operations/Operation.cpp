/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Operation.h"

namespace app
{
Operation::Operation(Programmer &programmer, const SectorTableModel &model, const QString &name)
    : m_name{name}
    , m_programmer{programmer}
    , m_sectorTableModel{model}
    , m_selectedSectors{model.getSelectedItems()}
{
}


const QString &Operation::getName() const
{
    return m_name;
}


Programmer &Operation::getProgrammer() const
{
    return m_programmer;
}


const SectorTableModel &Operation::getSectorTableModel() const
{
    return m_sectorTableModel;
}


const std::vector<SectorTableModel::Sector> &Operation::getSelectedSectors() const
{
    return m_selectedSectors;
}

} // namespace app
