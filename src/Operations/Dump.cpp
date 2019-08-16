/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Dump.h"

#include <cmath>

namespace app
{
Dump::Dump(app::Programmer *programmer, app::SectorTableModel *model, const QString &fileName)
    : Operation{programmer, model, "Считывание"}
    , m_file{fileName}
{
}


std::optional<QString> Dump::validate()
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

    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return QString("Невозможно записать в файл %1").arg(m_file.fileName());
    }

    return std::nullopt;
}


void Dump::run()
{
    const size_t chunkSize = 1024;

    std::list<std::vector<uint8_t>> chunks;

    const auto &[begin, total] = m_range;

    for (auto i = begin; i < begin + total; ++i)
    {
        const auto current = i - begin;
        const auto progressString =
            QString("Скопировано байт: %1 из %2").arg(current * chunkSize).arg(total * chunkSize);

        emit notifyProgress(static_cast<int>(total), static_cast<int>(current), progressString);

        chunks.emplace_back();
        getProgrammer()->readData(chunks.back(), i * chunkSize, chunkSize);
    }

    auto current = 0;
    for (const auto &chunk : chunks)
    {
        ++current;

        emit notifyProgress(static_cast<int>(m_range.second), current,
                            QString{"Запись в файл %1 / %2"}.arg(current).arg(chunks.size()));

        m_file.write(reinterpret_cast<const char *>(chunk.data()), static_cast<qint64>(chunk.size()));
    }

    m_file.close();

    emit notifyComplete();
}


QString Dump::getDescription() const
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

    return QString("Считать данные из секторов:\n\t%1\nВ файл:\n\t%2")
        .arg(selectedSectorsString)
        .arg(m_file.fileName());
}

} // namespace app
