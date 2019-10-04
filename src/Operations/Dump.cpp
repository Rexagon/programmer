/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Dump.h"

#include <cmath>

namespace app
{
Dump::Dump(Programmer &programmer, const SectorTableModel &model, const QString &fileName)
    : Operation{programmer, model, "Считывание"}
    , m_file{fileName}
{
}


std::optional<QString> Dump::validate()
{
    if (getSelectedSectors().empty())
    {
        return QString("Не выбрано ни одного сектора");
    }

    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return QString("Невозможно записать в файл %1").arg(m_file.fileName());
    }

    return std::nullopt;
}


void Dump::run()
{
    const size_t chunkSize = 1024u;

    // Считывание в память
    std::list<std::vector<uint8_t>> chunks;
    const auto sectors = getSectorTableModel().getItems();

    // Считаем сколько надо всего прочитать
    size_t total = 0;
    for (const auto &sector : sectors)
    {
        total += sector.size;
    }

    // Считываем память
    size_t current = 0;
    for (const auto &sector : sectors)
    {
        if (!sector.selected)
        {
            chunks.emplace_back(sector.size, 0xffu);
            current += sector.size;
            continue;
        }

        for (auto address = sector.address; address < sector.address + sector.size; address += chunkSize)
        {
            const auto progressString = QString("Скопировано байт: %L1 из %L2").arg(current).arg(total);
            emit notifyProgress(static_cast<int>(total), static_cast<int>(current), progressString);

            chunks.emplace_back();
            getProgrammer().readData(chunks.back(), address, chunkSize);
            current += chunkSize;
        }
    }

    // Запись в файл
    int currentChunk = 0;
    for (const auto &chunk : chunks)
    {
        printf("Chunk size: %ull\n", chunk.size());

        const auto progressString = QString("Запись в файл: %1 / %2").arg(currentChunk + 1).arg(chunks.size());
        emit notifyProgress(static_cast<int>(chunks.size()), currentChunk, progressString);
        ++currentChunk;

        m_file.write(reinterpret_cast<const char *>(chunk.data()), static_cast<qint64>(chunk.size()));
    }

    m_file.close();


    // Готово
    emit notifyProgress(1, 1, "Готово");
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
