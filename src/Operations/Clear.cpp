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
Clear::Clear(SectorTableModel *model)
    : Operation{model, "Очистка"}
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
    for (int i = 1; i <= 100; ++i)
    {
        std::this_thread::sleep_for(0.05s);

        emit notifyProgress(i, QString("Проверка: %1/100").arg(i));
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
