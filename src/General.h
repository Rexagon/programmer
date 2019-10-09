#ifndef PROGRAMMER_GENERAL_H
#define PROGRAMMER_GENERAL_H

#include <QString>

namespace app
{
enum class ConnectionState
{
    DISCONNECTED,
    CONNECTING,
    CONNECTED
};


enum class ApplicationState
{
    DISCONNECTED,
    CONNECTED
};

} // namespace app

#endif // PROGRAMMER_GENERAL_H
