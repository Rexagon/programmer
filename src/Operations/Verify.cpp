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
Verify::Verify(Programmer &programmer, const SectorPresetsModel &model, const QString &fileName)
    : Operation{programmer, model, "Проверка"}
    , m_file{fileName}
{
}


std::optional<QString> Verify::validate()
{
    const auto &selectedPresets = getSelectedPresets();

    if (selectedPresets.empty())
    {
        return QString("Не выбрано ни одного сектора");
    }

    if (!m_file.open(QIODevice::ReadOnly))
    {
        return QString("Невозможно открыть файл %1").arg(m_file.fileName());
    }

    return std::nullopt;
}


void Verify::run()
{
    const auto &selectedPresets = getSelectedPresets();
    std::vector<Differences> differences;

    // Чтение файла
    emit notifyProgress(0, 0, "Чтение файла");
    const auto data = m_file.readAll();

    // Проверка групп секторов
    for (const auto &item : selectedPresets)
    {
        const auto &preset = item.first;

        const auto result = verifyPreset(preset, data);
        if (result)
        {
            differences.emplace_back(*result);
        }
    }

    // Готово
    if (differences.empty())
    {
        emit notifyProgress(1, 1, "Данные в секторах совпадают с данными в файле");
    }
    else
    {
        QString resultText;

        for (const auto &item : differences)
        {
            QStringList sectorsText;
            for (const auto &sectorIndex : item.second)
            {
                sectorsText.append(QString("SA%1").arg(item.first.sectors[sectorIndex].number));
            }

            resultText +=
                QString("\t%1. Отличаются сектора:\n\t\t%2\n").arg(item.first.name).arg(sectorsText.join(", "));
        }

        emit notifyProgress(1, 1, QString("Найдены различия:\n%1").arg(resultText));
    }
    emit notifyComplete();
}


QString Verify::getDescription() const
{
    const auto &selectedPresets = getSelectedPresets();

    QString presetsString;
    for (const auto &item : selectedPresets)
    {
        const auto &preset = item.first;
        presetsString += QString("\t%1\n").arg(preset.name);
    }

    return QString("Сравнить файл:\n\t%1\nС данными в секторах:\n%2").arg(m_file.fileName()).arg(presetsString);
}


std::optional<Verify::Differences> Verify::verifyPreset(const SectorPresetsModel::Preset &preset,
                                                        const QByteArray &data)
{
    Differences differences{preset, {}};

    const size_t chunkSize = 1024;
    std::vector<uint8_t> chunk{};

    const auto &sectors = preset.sectors;

    size_t sectorsSize = 0;
    for (const auto &sector : sectors)
    {
        sectorsSize += sector.size;
    }

    const auto dataSize = std::min(static_cast<size_t>(data.size()), sectorsSize);

    const auto begin = sectors.front().address;
    const auto end = begin + dataSize;

    // Проверка
    for (size_t i = 0; i < sectors.size(); ++i)
    {
        const auto &sector = sectors[i];

        const auto sectorEnd = sector.address + sector.size;

        for (auto address = sector.address; address < sectorEnd && address < end; address += chunkSize)
        {
            const auto globalCurrent = address - begin;
            const auto localCurrent = address - sector.address;

            const auto progressString =
                QString("%1. Проверено байт: %L2 из %L3").arg(preset.name).arg(globalCurrent).arg(dataSize);

            getProgrammer().readData(chunk, address, chunkSize);

            const auto mismatch = compare(data.data() + localCurrent, chunk.data(), chunkSize);
            if (mismatch)
            {
                differences.second.emplace_back(i);
            }
        }
    }

    return differences.second.empty() ? std::nullopt : std::optional{differences};
}


} // namespace app
