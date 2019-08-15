/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Programmer.h"

#include <iostream>

#include <sitl/commands/List.h>
#include <sitl/commands/Mwr.h>

namespace
{
constexpr auto VERSION_REG = 0x000000u;

constexpr auto SERVICE_REG = 0x080000u;
constexpr auto ADDRESS_REG = 0x080002u;

constexpr auto BIVK_DATA_BEGIN = 0x040000u;
constexpr auto BIVK_DATA_END = 0x07FFFFu;
} // namespace


namespace app
{
Programmer::Programmer(const std::string &port, unsigned int baudRate)
    : m_connection{port, baudRate, true}
{
    m_connection.setResponseTimeout(2);

    m_connection.execute<sitl::cmds::List>();

    m_connection.execute<sitl::cmds::Mwr<uint32_t, uint16_t>>(SERVICE_REG, 0xFFFFu);
}


Programmer::~Programmer()
{
    m_connection.execute<sitl::cmds::Mwr<uint32_t, uint16_t>>(SERVICE_REG, 0x0000u);
}


} // namespace app
