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
    const auto base = extensionIndex < 0 ? m_fileName : m_fileName.left(extensionIndex - 1);
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
        presetsString += QString("\t%1 в %2\n").arg(preset.name).arg(m_files[index]->fileName());
    }

    return QString("Считывание:\n%1").arg(presetsString);
}


void Dump::dumpPreset(const SectorPresetsModel::Preset &preset, size_t index)
{
    const auto chunkSize = 1024;

    std::list<std::vector<uint8_t>> chunks;

    // Считаем сколько надо всего прочитать
    size_t total = 0;
    for (const auto &sector : preset.sectors)
    {
        total += sector.size;
    }

    // Считываем память
    size_t current = 0;
    for (const auto &sector : preset.sectors)
    {
        for (auto address = sector.address; address < sector.address + sector.size; address += chunkSize)
        {
            const auto progressString =
                QString("%1. Скопировано байт: %L2 из %L3").arg(preset.name).arg(current).arg(total);

            emit notifyProgress(static_cast<int>(total), static_cast<int>(current), progressString);

            chunks.emplace_back();
            getProgrammer().readData(chunks.back(), address, chunkSize);
            current += chunkSize;
        }
    }

    // Запись в файл
    auto &file = m_files[index];

    int currentChunk = 0;
    for (const auto &chunk : chunks)
    {
        const auto progressString =
            QString("%1. Запись в файл: %2 / %3").arg(preset.name).arg(currentChunk + 1).arg(chunks.size());

        emit notifyProgress(static_cast<int>(chunks.size()), currentChunk, progressString);
        ++currentChunk;

        file->write(reinterpret_cast<const char *>(chunk.data()), static_cast<qint64>(chunk.size()));
    }

    file->close();
}

} // namespace app
