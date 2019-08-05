/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "Verify.h"

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

namespace app
{
Verify::Verify(app::SectorTableModel *model, const QString &fileName)
    : Operation{model, "Проверка"}
    , m_fileName{fileName}
{
}


std::optional<QString> Verify::validate()
{
    bool hasSelectedSectors = false;
    const auto sectors = getModel()->getItems();
    for (const auto &sector : sectors)
    {
        if (sector.selected)
        {
            hasSelectedSectors = true;
            break;
        }
    }

    if (!hasSelectedSectors)
    {
        return QString("Не выбрано ни одного сектора");
    }

    // TODO: check file and sectors size

    return std::nullopt;
}


void Verify::run()
{
    for (int i = 1; i <= 100; ++i)
    {
        std::this_thread::sleep_for(0.05s);

        emit notifyProgress(i, QString("Проверка: %1/100").arg(i));
    }

    emit notifyComplete();
}


QString Verify::getDescription() const
{
    const auto sectors = getModel()->getItems();
    QString selectedSectorsString = "";

    for (const auto &sector : sectors)
    {
        if (!sector.selected)
        {
            continue;
        }

        if (!selectedSectorsString.isEmpty())
        {
            selectedSectorsString += ", ";
        }

        selectedSectorsString += QString("SA%1").arg(sector.number);
    }

    return QString("Сравнить файл:\n\t%1\nС данными в секторах:\n\t%2").arg(m_fileName, selectedSectorsString);
}


} // namespace app