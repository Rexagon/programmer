#ifndef PROGRAMMER_GENERAL_H
#define PROGRAMMER_GENERAL_H

#include <QString>

namespace app
{
enum class ViewMode
{
    COMPACT,
    EXTENDED
};


ViewMode operator!(const ViewMode &m);


constexpr const char *toString(const ViewMode &m)
{
    switch (m)
    {
        case ViewMode::COMPACT:
            return "Простой режим";

        case ViewMode::EXTENDED:
            return "Расширенный режим";

        default:
            return "";
    }
}


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
