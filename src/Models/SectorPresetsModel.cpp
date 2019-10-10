/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "SectorPresetsModel.h"

#include <array>

#include "../Settings.h"

namespace
{
using Sector = app::SectorPresetsModel::Sector;

const auto SECTOR_SIZE = 64 * 1024;

const std::array<Sector, 20> SECTORS = {
    Sector{0, 0x00000, SECTOR_SIZE / 4}, // SA0
    Sector{1, 0x04000, SECTOR_SIZE / 8}, // SA1
    Sector{2, 0x06000, SECTOR_SIZE / 8}, // SA2
    Sector{3, 0x08000, SECTOR_SIZE / 2}, // SA3
    Sector{4, 0x10000, SECTOR_SIZE},     // SA4
    Sector{5, 0x20000, SECTOR_SIZE},     // SA5
    Sector{6, 0x30000, SECTOR_SIZE},     // SA6
    Sector{7, 0x40000, SECTOR_SIZE},     // SA7
    Sector{8, 0x50000, SECTOR_SIZE},     // SA8
    Sector{9, 0x60000, SECTOR_SIZE},     // SA9
    Sector{10, 0x70000, SECTOR_SIZE},    // SA10
    Sector{11, 0x80000, SECTOR_SIZE},    // SA11
    Sector{12, 0x90000, SECTOR_SIZE},    // SA12
    Sector{13, 0xA0000, SECTOR_SIZE},    // SA13
    Sector{14, 0xB0000, SECTOR_SIZE},    // SA14
    Sector{15, 0xC0000, SECTOR_SIZE},    // SA15
    Sector{16, 0xD0000, SECTOR_SIZE},    // SA16
    Sector{17, 0xE0000, SECTOR_SIZE},    // SA17
    Sector{18, 0xF0000, SECTOR_SIZE},    // SA18
};

} // namespace

namespace app
{
SectorPresetsModel::SectorPresetsModel()
    : m_presets{
          Preset{QString{"Копия 1 (Сектора SA0, SA1, SA2, SA3)"},
                 {SECTORS[0], SECTORS[1], SECTORS[2], SECTORS[3]},
                 Settings::getInstance().loadPresetState(0)},
          Preset{QString{"Копия 2 (Сектор SA11)"}, {SECTORS[11]}, Settings::getInstance().loadPresetState(1)},
      }
{
}


void SectorPresetsModel::setPresetSelected(size_t index, bool selected)
{
    assert(index < m_presets.size());

    m_presets[index].selected = selected;
    Settings::getInstance().savePresetState(index, selected);
}


const std::vector<SectorPresetsModel::Preset> &SectorPresetsModel::getPresets() const
{
    return m_presets;
}


std::vector<std::pair<SectorPresetsModel::Preset, size_t>> SectorPresetsModel::getSelectedPresets() const
{
    std::vector<std::pair<Preset, size_t>> result;

    for (size_t i = 0; i < m_presets.size(); ++i)
    {
        if (m_presets[i].selected)
        {
            result.emplace_back(m_presets[i], i);
        }
    }

    return result;
}

} // namespace app
