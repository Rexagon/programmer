/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Program.h"

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

namespace app
{
Program::Program(SectorTableModel *model, const QString &fileName)
    : Operation{"Запись прошивки"}
    , m_model{model}
    , m_fileName{fileName}
{
}


std::optional<QString> Program::check()
{
    assert(m_model != nullptr);

    bool hasSelectedSectors = false;
    const auto sectors = m_model->getItems();
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


void Program::run()
{
    assert(m_model != nullptr);

    for (int i = 1; i <= 100; ++i)
    {
        std::this_thread::sleep_for(0.05s);

        emit notifyProgress(i, QString("Программирование: %1/100").arg(i));
    }

    emit notifyComplete();
}


QString Program::getDescription() const
{
    assert(m_model != nullptr);

    const auto sectors = m_model->getItems();
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

    return QString("Запись файла:\n\t%1\nВ секторы:\n\t%2").arg(m_fileName, selectedSectorsString);
}

} // namespace app
