/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Dump.h"

#include <cmath>

namespace app
{
Dump::Dump(Programmer &programmer, const SectorPresetsModel &model, const QString &fileName)
    : Operation{programmer, model, "Считывание"}
    , m_fileName{fileName}
    , m_files{}
{
}


std::optional<QString> Dump::validate()
{
    const auto &selectedPresets = getSelectedPresets();

    if (selectedPresets.empty())
    {
        return QString("Не выбрано ни одного сектора");
    }

    const auto extensionIndex = m_fileName.lastIndexOf(".");
    const auto base = extensionIndex < 0 ? m_fileName : m_fileName.left(extensionIndex);
    const auto extension = extensionIndex < 0 ? QString{} : m_fileName.mid(extensionIndex + 1);

    for (const auto &item : selectedPresets)
    {
        const auto index = item.second;

        const auto fileName = QString{"%1_%2.%3"}.arg(base).arg(index).arg(extension);

        auto &file = m_files.emplace_back(std::make_unique<QFile>(fileName));

        if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return QString("Невозможно записать в файл %1").arg(fileName);
        }
    }

    return std::nullopt;
}


void Dump::run()
{
    const auto &selectedPresets = getSelectedPresets();

    for (const auto &[preset, index] : selectedPresets)
    {
        dumpPreset(preset, index);
    }

    // Готово
    emit notifyProgress(1, 1, "Готово");
    emit notifyComplete();
}


QString Dump::getDescription() const
{
    const auto &selectedPresets = getSelectedPresets();

    QString presetsString;
    for (const auto &[preset, index] : selectedPresets)
    {
        presetsString += QString("    %1\n    в %2\n\n").arg(preset.name).arg(m_files[index]->fileName());
    }

    return QString("Считывание:\n%1").arg(presetsString);
}


void Dump::dumpPreset(const SectorPresetsModel::Preset &preset, size_t index)
{
    const auto chunkSize = 1024;

    // Считаем сколько надо всего прочитать
    const auto &sectors = preset.sectors;

    size_t dataSize = 0;
    for (const auto &sector : sectors)
    {
        dataSize += sector.size;
    }

    const auto begin = sectors.front().address;
    const auto end = begin + dataSize;

    std::list<std::vector<uint8_t>> chunks;

    // Считываем память
    for (auto address = begin; address < end; address += chunkSize)
    {
        checkCancelled();

        const auto current = address - begin;
        const auto progressString =
            QString("%1\nСкопировано байт: %L2 из %L3").arg(preset.name).arg(current).arg(dataSize);

        emit notifyProgress(static_cast<int>(dataSize), static_cast<int>(current), progressString);

        auto &chunk = chunks.emplace_back();
        getProgrammer().readData(chunk, address, chunkSize);
    }

    // Запись в файл
    auto &file = m_files[index];

    int currentChunk = 0;
    for (const auto &chunk : chunks)
    {
        checkCancelled();

        const auto progressString =
            QString("%1\nЗапись в файл: %2 / %3").arg(preset.name).arg(currentChunk + 1).arg(chunks.size());

        emit notifyProgress(static_cast<int>(chunks.size()), currentChunk, progressString);
        ++currentChunk;

        file->write(reinterpret_cast<const char *>(chunk.data()), static_cast<qint64>(chunk.size()));
    }

    file->close();
}

} // namespace app
