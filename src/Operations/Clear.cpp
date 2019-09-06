/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Clear.h"

#include <chrono>

namespace app
{
Clear::Clear(Programmer &programmer, const SectorTableModel &model)
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

    // Очистка секторов
    for (size_t i = 0; i < selectedSectors.size(); ++i)
    {
        emit notifyProgress(static_cast<int>(selectedSectors.size()), static_cast<int>(i),
                            QString("Очищено секторов: %1 из %2").arg(i).arg(selectedSectors.size()));

        getProgrammer().clearSector(selectedSectors[i]);
    }


    // Готово
    emit notifyProgress(1, 1, "Готово");
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
