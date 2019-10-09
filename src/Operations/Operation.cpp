/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Operation.h"

namespace app
{
Operation::Operation(Programmer &programmer, const SectorPresetsModel &model, const QString &name)
    : m_name{name}
    , m_programmer{programmer}
    , m_sectorPresetsModel{model}
    , m_selectedPresets{model.getSelectedPresets()}
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


const SectorPresetsModel &Operation::getSectorPresetsModel() const
{
    return m_sectorPresetsModel;
}

const std::vector<std::pair<SectorPresetsModel::Preset, size_t>> &Operation::getSelectedPresets() const
{
    return m_selectedPresets;
}

} // namespace app
