#ifndef PROGRAMMER_SRC_MODELS_PROGRAMMER_H_
#define PROGRAMMER_SRC_MODELS_PROGRAMMER_H_

#include <sitl/Connection.h>

namespace app
{
class Programmer final
{
public:
    struct TimingValue
    {
        uint16_t value;
    };

    explicit Programmer(const std::string &port, unsigned int baudRate);
    ~Programmer();

    void reset();

    void setBuffersEnabled(bool enabled);
    void setWritingTimings(const TimingValue &setup, const TimingValue &active, const TimingValue &hold);
    void setReadingTimings(const TimingValue &setup, const TimingValue &active, const TimingValue &hold);

    void applyConfiguration();

    const std::string &getDescription() const;

private:
    sitl::Connection m_connection;

    std::string m_description{};
    uint16_t m_serviceReg = 0x0000u;
};

constexpr Programmer::TimingValue operator""_T(unsigned long long value)
{
    return Programmer::TimingValue{static_cast<uint8_t>(value)};
}

} // namespace app

#endif // PROGRAMMER_SRC_MODELS_PROGRAMMER_H_
