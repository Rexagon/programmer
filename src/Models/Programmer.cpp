/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Programmer.h"

#include <cassert>

#include <sitl/commands/Iden.h>
#include <sitl/commands/List.h>
#include <sitl/commands/Mrd.h>
#include <sitl/commands/Mwr.h>

using namespace sitl::cmds;

namespace
{
constexpr uint32_t VERSION_REG = 0x00000000u;

constexpr uint32_t SERVICE_REG = 0x00080000u;
constexpr uint32_t ADDRESS_REG = 0x00080002u;

constexpr uint32_t BIVK_DATA_BEGIN = 0x00040000u;
constexpr uint32_t BIVK_DATA_END = 0x0007FFFFu;

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
    bool check(const app::Programmer::TimingValue &timing) const
    {
        return timing.value >= min && timing.value <= max;
    }

    void write(uint16_t &target, const app::Programmer::TimingValue &timing) const
    {
        assert(check(timing));
        setBits(target, static_cast<uint16_t>(timing.value + 4u - static_cast<uint16_t>(max - min)), 0x0003u, offset);
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
    : m_connection{port, baudRate, false}
{
    // Проверяем соединение
    m_connection.setResponseTimeout(2);
    m_connection.execute<sitl::cmds::List>();

    // Получаем описание устройства
    // m_description = m_connection.execute<sitl::cmds::Iden>();

    reset();
    disableProgramming();
}


Programmer::~Programmer()
{
    m_connection.execute<sitl::cmds::Mwr<uint32_t, uint16_t>>(SERVICE_REG, 0x0000u);
}


void Programmer::reset()
{
    // Устанавливаем тайминги
    setWritingTimings(4_T, 6_T, 4_T);
    setReadingTimings(4_T, 7_T, 4_T);

    // Включаем буффер
    setBuffersEnabled(true);

    // Режим сброса на бивке
    setBits(m_serviceReg, 1u, 0x0001u, sr::EXT_RES_O);

    // Применяем конфигурацию
    applyConfiguration();

    // Отключаем режим программирования
    disableProgramming();
}


void Programmer::readData(std::vector<uint8_t> &data, size_t begin, const size_t size)
{
    if (begin + size > BIVK_DATA_END - BIVK_DATA_BEGIN)
        throw std::logic_error{"Невозможно считать блок данных"};

    data.reserve(size);
    for (size_t address = begin; address < begin + size; ++address)
    {
        data.emplace_back(m_connection.execute<Mrd<uint32_t, uint8_t>>(BIVK_DATA_BEGIN + address));
    }
}


void Programmer::writeData(const uint8_t *data, const size_t begin, const size_t size)
{
    if (!m_isProgrammingEnabled)
        throw std::runtime_error{"Режим программирования выключен"};

    if (begin + size > BIVK_DATA_END - BIVK_DATA_BEGIN)
        throw std::logic_error{"Невозможно записать блок данных"};

    for (size_t i = 0; i < size; ++i)
    {
        m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN, 0xA0u);
        m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN + begin + i, data[i]);
    }
}


void Programmer::enableProgramming()
{
    m_isProgrammingEnabled = true;
    m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN + 0xAAAu, 0xAAu);
    m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN + 0x555u, 0x55u);
    m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN + 0xAAAu, 0x20u);
}


void Programmer::disableProgramming()
{
    m_isProgrammingEnabled = false;
    m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN, 0x90u);
    m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN, 0x00u);
}


void Programmer::clearSector(const app::SectorTableModel::Sector &sector)
{
    using namespace sitl::cmds;

    m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN + 0xAAAu, 0xAAu);
    m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN + 0x555u, 0x55u);

    m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN + 0xAAAu, 0x80u);
    m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN + 0xAAAu, 0xAAu);

    m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN + 0x555u, 0x55u);
    m_connection.execute<Mwr<uint32_t, uint8_t>>(BIVK_DATA_BEGIN + sector.address * 1024, 0x30u);
}


void Programmer::setBuffersEnabled(bool enabled)
{
    setBits(m_serviceReg, enabled, 0x0001u, sr::BUS_OE);
}


void Programmer::setWritingTimings(const Programmer::TimingValue &setup,
                                   const Programmer::TimingValue &active,
                                   const Programmer::TimingValue &hold)
{
    using namespace timings;

    WRITE_SETUP.write(m_serviceReg, setup);
    WRITE_ACTIVE.write(m_serviceReg, active);
    WRITE_HOLD.write(m_serviceReg, hold);
}


void Programmer::setReadingTimings(const app::Programmer::TimingValue &setup,
                                   const app::Programmer::TimingValue &active,
                                   const app::Programmer::TimingValue &hold)
{
    using namespace timings;

    READ_SETUP.write(m_serviceReg, setup);
    READ_ACTIVE.write(m_serviceReg, active);
    READ_HOLD.write(m_serviceReg, hold);
}


const std::string &Programmer::getDescription() const
{
    return m_description;
}


void Programmer::applyConfiguration()
{
    m_connection.execute<sitl::cmds::Mwr<uint32_t, uint16_t>>(SERVICE_REG, m_serviceReg);
}

} // namespace app
