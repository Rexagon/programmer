/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Operation.h"

namespace app
{
Operation::Operation(const QString &name)
    : m_name{name}
{
}


const QString &Operation::getName() const
{
    return m_name;
}

} // namespace app
