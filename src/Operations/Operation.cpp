/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Operation.h"

namespace app
{
Operation::Operation(SectorTableModel *model, const QString &name)
    : m_model{model}
    , m_name{name}
{
    assert(m_model != nullptr);
}


const QString &Operation::getName() const
{
    return m_name;
}


SectorTableModel *Operation::getModel() const
{
    return m_model;
}


} // namespace app
