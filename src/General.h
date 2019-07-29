#ifndef PROGRAMMER_GENERAL_H
#define PROGRAMMER_GENERAL_H

namespace app
{
enum class ViewMode
{
    COMPACT,
    EXTENDED
};

ViewMode operator!(const ViewMode &m);

enum class ConnectionState
{
    DISCONNECTED,
    CONNECTING,
    CONNECTED
};

enum class ApplicationState
{
    DISCONNECTED,
    CONNECTED,
    FILE_SELECTED
};

} // namespace app

#endif // PROGRAMMER_GENERAL_H
