#ifndef PROGRAMMER_SRC_SETTINGS_H_
#define PROGRAMMER_SRC_SETTINGS_H_

#include <QSettings>

namespace app
{
class Settings
{
public:
    static Settings &getInstance();

    void saveSelectedSerialPort(const QString &portName);
    QString loadSelectedSerialPort() const;

    void savePresetState(size_t index, bool selected);
    bool loadPresetState(size_t index) const;

public:
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;
    Settings(Settings &&) noexcept = delete;
    Settings &operator=(Settings &&) noexcept = delete;

private:
    explicit Settings();

    QSettings m_settings;
};

} // namespace app

#endif // PROGRAMMER_SRC_SETTINGS_H_
