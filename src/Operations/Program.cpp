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
Program::Program(Programmer &programmer, const SectorPresetsModel &model, const QString &fileName)
    : Operation{programmer, model, "Запись прошивки"}
    , m_file{fileName}
{
}


std::optional<QString> Program::validate()
{
    const auto &selectedPresets = getSelectedPresets();

    if (selectedPresets.empty())
    {
        return QString("Не выбрано ни одного сектора");
    }

    if (!m_file.open(QIODevice::ReadOnly | QIODevice::ExistingOnly))
    {
        return QString("Невозможно открыть файл прошивки:\n%1").arg(m_file.fileName());
    }

    return std::nullopt;
}


void Program::run()
{
    const auto &selectedPresets = getSelectedPresets();

    // Очистка секторов
    for (const auto &item : selectedPresets)
    {
        const auto &preset = item.first;
        const auto &sectors = preset.sectors;

        for (size_t i = 0; i < sectors.size(); ++i)
        {
            checkCancelled();

            emit notifyProgress(static_cast<int>(sectors.size()), static_cast<int>(i),
                                QString("%1\nОчищено секторов: %2 из %3").arg(preset.name).arg(i).arg(sectors.size()));

            getProgrammer().clearSector(sectors[i]);
        }
    }

    // Ожидание очистки секторов
    std::this_thread::sleep_for(std::chrono::seconds(1));

    //
    checkCancelled();

    const auto disableProgramming = [this]() {
        emit notifyProgress(0, 0, "Выключение режима программирования");
        getProgrammer().disableProgramming();
    };

    try
    {
        // Включение режима программирования
        emit notifyProgress(0, 0, "Включение режима программирования");
        getProgrammer().enableProgramming();

        //
        checkCancelled();

        // Ожидание включения режима программирования
        std::this_thread::sleep_for(std::chrono::seconds(1));

        //
        checkCancelled();

        // Чтение файла с прошивкой
        emit notifyProgress(0, 0, "Чтение файла");
        const auto data = m_file.readAll();

        //
        checkCancelled();

        // Программирование
        for (const auto &item : selectedPresets)
        {
            const auto &preset = item.first;
            programPreset(preset, data);
        }

        // Выключение режима программирования
        disableProgramming();
    }
    catch (const Operation::CancelledException &e)
    {
        // Выключение режима программирования
        disableProgramming();

        //
        throw e;
    }

    // Готово
    emit notifyProgress(1, 1, "Готово");
    emit notifyComplete();
}


QString Program::getDescription() const
{
    const auto &selectedPresets = getSelectedPresets();

    QString presetsString;
    for (const auto &item : selectedPresets)
    {
        const auto &preset = item.first;
        presetsString += QString("    %1\n").arg(preset.name);
    }

    return QString("Запись прошивки:\n    %1\nВ секторы:\n%2").arg(m_file.fileName()).arg(presetsString);
}


void Program::programPreset(const app::SectorPresetsModel::Preset &preset, const QByteArray &data)
{
    const size_t chunkSize = 1024;

    const auto &sectors = preset.sectors;

    size_t sectorsSize = 0;
    for (const auto &sector : sectors)
    {
        sectorsSize += sector.size;
    }

    const auto dataSize = std::min(static_cast<size_t>(data.size()), sectorsSize);

    const auto begin = sectors.front().address;
    const auto end = begin + dataSize;

    for (auto address = begin; address < end; address += chunkSize)
    {
        //
        checkCancelled();

        const auto current = address - begin;
        const auto progressString =
            QString("%1\nЗаписано байт: %L2 из %L3").arg(preset.name).arg(current).arg(dataSize);

        emit notifyProgress(static_cast<int>(dataSize), static_cast<int>(current), progressString);

        getProgrammer().writeData(data.data() + current, address, std::min(chunkSize, dataSize - current));
    }
}

} // namespace app
