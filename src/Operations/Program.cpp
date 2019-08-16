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
Program::Program(Programmer *programmer, SectorTableModel *model, const QString &fileName)
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

    // TODO: придумать что делать если выбраны области для ПО1 и ПО2
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
    const auto &programmer = getProgrammer();
    const auto &selectedSectors = getSelectedSectors();

    // Очистка секторов
    for (size_t i = 0; i < selectedSectors.size(); ++i)
    {
        emit notifyProgress(static_cast<int>(selectedSectors.size()), static_cast<int>(i),
                            QString("Очистка: %1/%2").arg(i).arg(selectedSectors.size()));

        getProgrammer()->clearSector(selectedSectors[i]);
    }

    // Чтение файла
    emit notifyProgress(0, 0, "Чтение файла");

    std::vector<uint8_t> data;
    data.resize(static_cast<size_t>(m_file.size()));
    m_file.read(reinterpret_cast<char *>(data.data()), data.size());

    size_t chunkSize = 1024;
    const size_t chunkCount = data.size() / chunkSize + static_cast<size_t>((data.size() % chunkSize) > 0);

    // Программирование
    emit notifyProgress(0, 0, "Включение режима программирования");
    programmer->enableProgramming();

    const auto &begin = m_range.first * 1024;

    for (size_t i = 0; i < chunkCount; ++i)
    {
        emit notifyProgress(static_cast<int>(chunkCount), static_cast<int>(i),
                            QString("Программирование: %1 / %2").arg(i * chunkSize).arg(chunkCount * chunkSize));


        // TODO: refactor
        const auto dataChunkSize = (i + 1) * chunkCount > data.size() ? data.size() % chunkSize : chunkSize;

        programmer->writeData(data.data() + i, begin + i, dataChunkSize);
    }

    emit notifyProgress(0, 0, "Выключение режима программирования");
    programmer->disableProgramming();

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
