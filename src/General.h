#ifndef PROGRAMMER_GENERAL_H
#define PROGRAMMER_GENERAL_H

#include <QSettings>
#include <QString>

#ifndef APP_VERSION
#define APP_VERSION "DEV"
#endif

#ifndef LOGGING_ENABLED
#define LOGGING_ENABLED 1
#endif

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
