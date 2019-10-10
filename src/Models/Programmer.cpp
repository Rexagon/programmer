/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Programmer.h"

#include <cassert>
#include <iostream>

#include <sitl/commands/Mrd.h>
#include <sitl/commands/Mwr.h>

#include "../General.h"

using namespace sitl::cmds;

namespace
{
void setBits(uint16_t &target, uint16_t value, uint16_t valueMask, uint16_t offset)
{
    const auto mask = static_cast<uint16_t>(~static_cast<uint16_t>(valueMask << offset));

    target &= mask;
    target |= static_cast<uint16_t>(static_cast<uint16_t>(value & valueMask) << offset);
}

namespace sr
{
constexpr uint16_t EXT_RES_O = 0u; // Сигнал сброса БИВК от программатора
constexpr uint16_t EXT_RES_I = 1u; // Сигнал сброса БИВК от самого БИВК
constexpr uint16_t BUS_OE = 2u;    // Включение буфферов
constexpr uint16_t RESET = 3u;     // Сигнал сброса программатора

} // namespace sr
} // namespace


namespace app
{
Programmer::Programmer(const std::string &port, unsigned int baudRate)
    : m_connection{port, baudRate, LOGGING_ENABLED}
{
    // Проверяем соединение
    m_connection.setResponseTimeout(2);

    // Получаем описание устройства
    // m_description = m_connection.execute<Iden>();

    // Получаем версию устройства
    m_version = m_connection.execute<Mrd<uint32_t, uint64_t>>(0x00000u);

    reset();
}


Programmer::~Programmer()
{
    setServiceReg(0x0000u);
    setAddressReg(0x0000u);
}


void Programmer::reset()
{
    // Включаем буффер
    setBuffersEnabled(true);

    // Устанавливаем тайминги
    setWritingTimings(WriteSetupTiming::T1, WriteActiveTiming::T3, WriteHoldTiming::T1);
    setReadingTimings(ReadSetupTiming::T1, ReadActiveTiming::T7, ReadHoldTiming::T2);

    // Применяем конфигурацию
    applyConfiguration();

    // Обнуляем адресный регистр
    setAddressReg(0x0000u, true);

    // Отключаем режим программирования
    disableProgramming();

    // На всякий случай выполняем команду Reset для flash
    writeData(0x00000u, 0xF0F0F0F0u);
}


void Programmer::readData(std::vector<uint8_t> &data, const size_t begin, const size_t size)
{
    if (m_isProgrammingEnabled)
    {
        printf("Unable to read in programming mode\n");
        throw std::runtime_error{"Чтение данных невозможно при включённом режиме программирования"};
    }

    if (begin + size > BIVK_DATA_END - BIVK_DATA_BEGIN)
    {
        printf("Unable to read block\n");
        throw std::logic_error{"Невозможно считать блок данных"};
    }

    data.clear();
    data.reserve(size);
    for (size_t address = begin; address < begin + size;)
    {
        using ReadType = uint64_t;

        const auto result = readData<ReadType>(address);

        for (size_t i = 0; i < sizeof(ReadType); ++i)
        {
            data.emplace_back(*(reinterpret_cast<const uint8_t *>(&result) + i));
        }

        address += sizeof(ReadType);
    }
}


void Programmer::writeData(const void *data, const size_t begin, const size_t size)
{
    if (!m_isProgrammingEnabled)
    {
        throw std::runtime_error{"Запись данных невозможна при выключенном режиме программирования"};
    }

    if (begin + size > BIVK_DATA_END - BIVK_DATA_BEGIN)
    {
        throw std::logic_error{"Невозможно записать блок данных"};
    }

    using WriteType = uint32_t;
    for (size_t i = 0; i * sizeof(WriteType) < size; ++i)
    {
        writeData(0x00000, 0xA0A0A0A0u);
        writeData(begin + i * sizeof(WriteType), *(reinterpret_cast<const WriteType *>(data) + i));
    }
}


void Programmer::enableProgramming()
{
    m_isProgrammingEnabled = true;

    writeData(0x00000u, 0xF0F0F0F0u);

    writeData(0x00AAAu << 2u, 0xAAAAAAAAu);
    writeData(0x00555u << 2u, 0x55555555u);
    writeData(0x00AAAu << 2u, 0x20202020u);
}


void Programmer::disableProgramming()
{
    m_isProgrammingEnabled = false;
    writeData(0x00000u, 0x90909090u);
    writeData(0x00000u, 0x00000000u);
}


void Programmer::clearSector(const SectorPresetsModel::Sector &sector)
{
    writeData(0x00AAAu << 2u, 0xAAAAAAAAu);
    writeData(0x00555u << 2u, 0x55555555u);
    writeData(0x00AAAu << 2u, 0x80808080u);
    writeData(0x00AAAu << 2u, 0xAAAAAAAAu);
    writeData(0x00555u << 2u, 0x55555555u);
    writeData(sector.address, 0x30303030u);
}


const std::string &Programmer::getDescription() const
{
    return m_description;
}


uint64_t Programmer::getVersion() const
{
    return m_version;
}


void Programmer::setBuffersEnabled(bool enabled)
{
    m_areBuffersEnabled = enabled;
}


void Programmer::setWritingTimings(WriteSetupTiming setup, WriteActiveTiming active, WriteHoldTiming hold)
{
    m_writingTimings = {setup, active, hold};
}


void Programmer::setReadingTimings(ReadSetupTiming setup, ReadActiveTiming active, ReadHoldTiming hold)
{
    m_readingTimings = {setup, active, hold};
}


void Programmer::applyConfiguration()
{
    uint16_t configuration = 0x0000u;

    setBits(configuration, true, 0b1u, sr::EXT_RES_O); // Всегда включаем режим сброса на бивке

    setBits(configuration, m_areBuffersEnabled, 0b1u, sr::BUS_OE);

    setBits(configuration, static_cast<uint16_t>(std::get<WriteSetupTiming>(m_writingTimings)), 0b11u, 4u);
    setBits(configuration, static_cast<uint16_t>(std::get<WriteActiveTiming>(m_writingTimings)), 0b11u, 6u);
    setBits(configuration, static_cast<uint16_t>(std::get<WriteHoldTiming>(m_writingTimings)), 0b11u, 8u);

    setBits(configuration, static_cast<uint16_t>(std::get<ReadSetupTiming>(m_readingTimings)), 0b11u, 10u);
    setBits(configuration, static_cast<uint16_t>(std::get<ReadActiveTiming>(m_readingTimings)), 0b11u, 12u);
    setBits(configuration, static_cast<uint16_t>(std::get<ReadHoldTiming>(m_readingTimings)), 0b11u, 14u);

    setServiceReg(configuration);
}


void Programmer::setServiceReg(const uint16_t data, const bool force)
{
    if (m_serviceReg != data || force)
    {
        m_connection.execute<Mwr<uint32_t, uint16_t>>(SERVICE_REG, data);
        m_serviceReg = data;
    }
}


void Programmer::setAddressReg(const uint16_t data, const bool force)
{
    if (m_addressReg != data || force)
    {
        m_connection.execute<Mwr<uint32_t, uint16_t>>(ADDRESS_REG, data);
        m_addressReg = data;
    }
}

} // namespace app
