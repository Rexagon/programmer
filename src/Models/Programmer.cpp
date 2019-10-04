/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Programmer.h"

#include <cassert>

#include <sitl/commands/Iden.h>
#include <sitl/commands/Mrd.h>
#include <sitl/commands/Mwr.h>

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

namespace timings
{
struct Range
{
    bool check(const uint8_t value) const
    {
        return value >= min && value <= max;
    }

    void write(uint16_t &target, const uint8_t value) const
    {
        assert(check(value));
        setBits(target, static_cast<uint16_t>(value + 4u - static_cast<uint16_t>(max - min)), 0x0003u, offset);
    }

    uint16_t offset;

    uint16_t min;
    uint16_t max;
};

constexpr auto WRITE_SETUP = Range{4u /* offset */, 1u /* min */, 4u /* max */};
constexpr auto WRITE_ACTIVE = Range{6u /* offset */, 3u /* min */, 6u /* max */};
constexpr auto WRITE_HOLD = Range{8u /* offset */, 1u /* min */, 4u /* max */};

constexpr auto READ_SETUP = Range{10u /* offset */, 1u /* min */, 4u /* max */};
constexpr auto READ_ACTIVE = Range{12u /* offset */, 4u /* min */, 7u /* max */};
constexpr auto READ_HOLD = Range{14u /* offset */, 2u /* min */, 4u /* max */};
} // namespace timings
} // namespace


namespace app
{
Programmer::Programmer(const std::string &port, unsigned int baudRate)
    : m_connection{port, baudRate, true}
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
    setWritingTimings(1, 3, 1);
    setReadingTimings(2, 7, 1);

    // Применяем конфигурацию
    applyConfiguration();

    // Обнуляем адресный регистр
    setAddressReg(0x0000u, true);

    // Отключаем режим программирования
    disableProgramming();

    // На всякий случай выполняем команду Reset для flash
    writeData(0x00000u, 0xF0u);
}


void Programmer::readData(std::vector<uint8_t> &data, const size_t begin, const size_t size)
{
    if (m_isProgrammingEnabled)
    {
        throw std::runtime_error{"Чтение данных невозможно при включённом режиме программирования"};
    }

    if (begin + size > BIVK_DATA_END - BIVK_DATA_BEGIN)
    {
        throw std::logic_error{"Невозможно считать блок данных"};
    }

    data.reserve(size);
    for (size_t address = begin; address < begin + size; ++address)
    {
        data.emplace_back(readData<uint8_t>(address));
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

    for (size_t i = 0; i < size; ++i)
    {
        writeData(0x00000, 0xA0A0A0A0u);
        writeData(begin + i, static_cast<const uint8_t *>(data)[i]);
    }
}


void Programmer::enableProgramming()
{
    m_isProgrammingEnabled = true;

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


void Programmer::clearSector(const app::SectorTableModel::Sector &sector)
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


void Programmer::setWritingTimings(const uint8_t setup, const uint8_t active, const uint8_t hold)
{
    using namespace timings;

    assert(WRITE_SETUP.check(setup) && WRITE_ACTIVE.check(active) && WRITE_HOLD.check(hold));
    m_writingTimings = {setup, active, hold};
}


void Programmer::setReadingTimings(const uint8_t setup, const uint8_t active, const uint8_t hold)
{
    using namespace timings;

    assert(READ_SETUP.check(setup) && READ_ACTIVE.check(active) && READ_HOLD.check(hold));
    m_readingTimings = {setup, active, hold};
}


void Programmer::applyConfiguration()
{
    using namespace timings;

    uint16_t configuration = 0x0000u;

    setBits(m_serviceReg, true, 0x0001u, sr::EXT_RES_O); // Всегда включаем режим сброса на бивке

    setBits(configuration, m_areBuffersEnabled, 0x0001u, sr::BUS_OE);

    WRITE_SETUP.write(configuration, std::get<0>(m_writingTimings));
    WRITE_ACTIVE.write(configuration, std::get<1>(m_writingTimings));
    WRITE_HOLD.write(configuration, std::get<2>(m_writingTimings));

    READ_SETUP.write(configuration, std::get<0>(m_readingTimings));
    READ_ACTIVE.write(configuration, std::get<1>(m_readingTimings));
    READ_HOLD.write(configuration, std::get<2>(m_readingTimings));

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
