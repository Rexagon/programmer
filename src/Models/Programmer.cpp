/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Programmer.h"

#include <cassert>
#include <iomanip>
#include <iostream>

#include <sitl/commands/Iden.h>
#include <sitl/commands/List.h>
#include <sitl/commands/Mwr.h>

namespace
{
constexpr auto VERSION_REG = 0x000000u;

constexpr auto SERVICE_REG = 0x080000u;
constexpr auto ADDRESS_REG = 0x080002u;

constexpr auto BIVK_DATA_BEGIN = 0x040000u;
constexpr auto BIVK_DATA_END = 0x07FFFFu;

void setBits(uint16_t &target, uint16_t value, uint16_t valueMask, uint16_t offset)
{
    const auto mask = static_cast<uint16_t>(~static_cast<uint16_t>(valueMask << offset));
    target &= mask;
    target |= static_cast<uint16_t>(static_cast<uint16_t>(value & valueMask) << offset);
}

namespace sr
{
constexpr uint16_t EXT_RES_O = 0u;
constexpr uint16_t EXT_RES_I = 1u;
constexpr uint16_t BUS_OE = 2u;
constexpr uint16_t RESET = 3u;

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
        setBits(target, timing.value, 0x0003u, offset);
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
    m_connection.execute<sitl::cmds::List>();

    // Получаем описание устройства
    //m_description = m_connection.execute<sitl::cmds::Iden>();

    reset();
}


Programmer::~Programmer()
{
    m_connection.execute<sitl::cmds::Mwr<uint32_t, uint16_t>>(SERVICE_REG, 0x0000u);
}


void Programmer::reset()
{
    // Устанавливаем тайминги
    setWritingTimings(2_T, 4_T, 3_T);
    setReadingTimings(1_T, 7_T, 2_T);

    // Включаем буффер
    setBuffersEnabled(true);

    // Применяем конфигурацию
    applyConfiguration();
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

    auto readHold = hold;
    ++readHold.value;
    READ_HOLD.write(m_serviceReg, readHold);
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
