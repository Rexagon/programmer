/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Operation.h"

namespace app
{
Operation::Operation(SectorTableModel *model, const QString &name)
    : m_name{name}
    , m_selectedSectors{(assert(model != nullptr), model->getSelectedItems())}
{
}


const QString &Operation::getName() const
{
    return m_name;
}


const std::vector<SectorTableModel::Sector> &Operation::getSelectedSectors() const
{
    return m_selectedSectors;
}

} // namespace app
