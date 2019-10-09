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
Program::Program(Programmer &programmer, const SectorTableModel &model, const QString &fileName)
    : Operation{programmer, model, "Запись прошивки"}
    , m_file{fileName}
{
}


std::optional<QString> Program::validate()
{
    const auto &selectedSectors = getSelectedSectors();

    if (selectedSectors.empty())
    {
        return QString("Не выбрано ни одного сектора");
    }

    m_range = {selectedSectors.front().address, 0};

    for (auto it = selectedSectors.begin(); it != selectedSectors.end(); ++it)
    {
        m_range.second += it->size;

        auto nextIt = it + 1;
        if (nextIt != selectedSectors.end() && it->number + 1 != nextIt->number)
        {
            return QString("Выбраны не последовательные сектора");
        }
    }

    if (!m_file.open(QIODevice::ReadOnly | QIODevice::ExistingOnly))
    {
        return QString("Невозможно открыть файл прошивки:\n%1").arg(m_file.fileName());
    }

    if (m_file.size() > m_range.second * 1024)
    {
        return QString("Слишком большой файл для записи");
    }

    return std::nullopt;
}


void Program::run()
{
    const size_t chunkSize = 1024;
    const auto &begin = m_range.first;

    const auto &selectedSectors = getSelectedSectors();

    // Очистка секторов
    for (size_t i = 0; i < selectedSectors.size(); ++i)
    {
        emit notifyProgress(static_cast<int>(selectedSectors.size()), static_cast<int>(i),
                            QString("Очищено секторов: %1 из %2").arg(i).arg(selectedSectors.size()));

        getProgrammer().clearSector(selectedSectors[i]);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Чтение файла
    emit notifyProgress(0, 0, "Чтение файла");

    const auto data = m_file.readAll();
    const auto dataSize = static_cast<size_t>(data.size());

    // Программирование
    emit notifyProgress(0, 0, "Включение режима программирования");
    getProgrammer().enableProgramming();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    for (auto address = begin; address < begin + dataSize; address += chunkSize)
    {
        const auto current = address - begin;
        const auto progressString = QString("Записано байт: %L1 из %L2").arg(current).arg(dataSize);

        emit notifyProgress(static_cast<int>(dataSize), static_cast<int>(current), progressString);

        getProgrammer().writeData(data.data() + current, address, std::min(chunkSize, dataSize - current));
    }

    emit notifyProgress(0, 0, "Выключение режима программирования");
    getProgrammer().disableProgramming();

    // Готово
    emit notifyProgress(1, 1, "Готово");
    emit notifyComplete();
}


QString Program::getDescription() const
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

    return QString("Запись файла:\n\t%1\nВ секторы:\n\t%2").arg(m_file.fileName(), selectedSectorsString);
}

} // namespace app
