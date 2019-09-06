/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "Verify.h"

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

namespace
{
std::optional<size_t> compare(const void *first, const void *second, const size_t size)
{
    const auto *firstPtr = static_cast<const uint8_t *>(first);
    const auto *secondPtr = static_cast<const uint8_t *>(second);

    for (size_t i = 0; i < size; ++i)
    {
        if (*(firstPtr++) - *(secondPtr++) != 0)
        {
            return i;
        }
    }

    return std::nullopt;
}

} // namespace


namespace app
{
Verify::Verify(Programmer &programmer, const SectorTableModel &model, const QString &fileName)
    : Operation{programmer, model, "Проверка"}
    , m_file{fileName}
{
}


std::optional<QString> Verify::validate()
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
            return QString("Выбраны непоследовательные сектора");
        }
    }

    if (!m_file.open(QIODevice::ReadOnly))
    {
        return QString("Невозможно открыть файл %1").arg(m_file.fileName());
    }

    if (m_range.second != m_file.size())
    {
        return QString("Размеры сравниваемого файла и выделенных секторов не совпадают");
    }

    return std::nullopt;
}


void Verify::run()
{
    const size_t chunkSize = 1024;
    std::vector<uint8_t> chunk{};

    const auto &begin = m_range.first;

    // Чтение файла
    emit notifyProgress(0, 0, "Чтение файла");

    const auto data = m_file.readAll();
    const auto dataSize = static_cast<size_t>(data.size());

    // Проверка
    for (auto address = begin; address < dataSize; address += chunkSize)
    {
        const auto current = address - begin;
        const auto progressString = QString("Проверено байт: %L1 из %L2").arg(current).arg(dataSize);

        getProgrammer().readData(chunk, address, chunkSize);

        const auto mismatch = compare(data.data() + current, chunk.data(), chunkSize);
        if (mismatch)
        {
            // Обнаружено несовпадение
            emit notifyProgress(1, 1,
                                QString("Данные в секторах не совпадают с данными в файле.\n"
                                        "Адрес первого несовпадения: %1")
                                    .arg(address + *mismatch, 5, 16));
            emit notifyComplete();
            return;
        }
    }

    // Готово
    emit notifyProgress(1, 1, "Данные в секторах совпадают с данными в файле");
    emit notifyComplete();
}


QString Verify::getDescription() const
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

    return QString("Сравнить файл:\n\t%1\nС данными в секторах:\n\t%2").arg(m_file.fileName(), selectedSectorsString);
}


} // namespace app
