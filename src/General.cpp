/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "General.h"

namespace app
{
ViewMode operator!(const ViewMode &m)
{
    return m == ViewMode::COMPACT ? ViewMode::EXTENDED : ViewMode::COMPACT;
}

} // namespace app
