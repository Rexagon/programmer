/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "Clear.h"

#include <chrono>
#include <thread>

#include "Models/SectorTableModel.h"

using namespace std::chrono_literals;

namespace app
{
Clear::Clear(Programmer *programmer, SectorTableModel *model)
    : Operation{programmer, model, "Очистка"}
{
}


std::optional<QString> Clear::validate()
{
    const auto selectedSectors = getSelectedSectors();
    if (selectedSectors.empty())
    {
        return QString("Не выбрано ни одного сектора");
    }

    return std::nullopt;
}


void Clear::run()
{
    const auto selectedSectors = getSelectedSectors();

    for (size_t i = 0; i < selectedSectors.size(); ++i)
    {
        getProgrammer()->clearSector(selectedSectors[i]);

        emit notifyProgress(static_cast<int>(selectedSectors.size()), static_cast<int>(i + 1),
                            QString("Очистка: %1/%2").arg(i + 1).arg(selectedSectors.size()));
    }

    emit notifyComplete();
}


QString Clear::getDescription() const
{
    const auto sectors = getSelectedSectors();
    QString selectedSectorsString = "";

    for (const auto &sector : sectors)
    {
        if (!selectedSectorsString.isEmpty())
        {
            selectedSectorsString += ", ";
        }

        selectedSectorsString += QString("SA%1").arg(sector.number);
    }

    return QString("Очистить сектора:\n\t%2").arg(selectedSectorsString);
}


} // namespace app
