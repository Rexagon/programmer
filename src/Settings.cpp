/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "Settings.h"

namespace
{
constexpr auto SELECTED_SERIAL_PORT = "programmer/selectedSerialPort";
constexpr auto SELECTED_PRESETS = "programmer/selectedPresets";

} // namespace

namespace app
{
Settings &Settings::getInstance()
{
    static Settings settings{};

    return settings;
}

Settings::Settings()
    : m_settings{"RC Module", "programmer"}
{
}


void Settings::saveSelectedSerialPort(const QString &portName)
{
    m_settings.setValue(SELECTED_SERIAL_PORT, portName);
}


QString Settings::loadSelectedSerialPort() const
{
    return m_settings.value(SELECTED_SERIAL_PORT).toString();
}


void Settings::savePresetState(size_t index, bool selected)
{
    m_settings.setValue(QString{"%1/%2"}.arg(SELECTED_PRESETS).arg(index), selected);
}


bool Settings::loadPresetState(size_t index) const
{
    return m_settings.value(QString{"%1/%2"}.arg(SELECTED_PRESETS).arg(index), false).toBool();
}

} // namespace app
