#ifndef PROGRAMMER_SRC_MODELS_PROGRAMMER_H_
#define PROGRAMMER_SRC_MODELS_PROGRAMMER_H_

#include <sitl/Connection.h>

#include "SectorTableModel.h"

namespace app
{
class Programmer final
{
    using Timings = std::tuple<uint8_t, uint8_t, uint8_t>;

public:
    explicit Programmer(const std::string &port, unsigned int baudRate);
    ~Programmer();

    void reset();

    void readData(std::vector<uint8_t> &data, size_t begin, size_t size);
    void writeData(const void *data, size_t begin, size_t size);

    void enableProgramming();
    void disableProgramming();

    void clearSector(const SectorTableModel::Sector &sector);

    void setBuffersEnabled(bool enabled);
    void setWritingTimings(uint8_t setup, uint8_t active, uint8_t hold);
    void setReadingTimings(uint8_t setup, uint8_t active, uint8_t hold);

    void applyConfiguration();

    const std::string &getDescription() const;

private:
    uint8_t readData(uint32_t address);
    void writeData(uint32_t address, uint8_t data);

    void setServiceReg(uint16_t data, bool force = false);
    void setAddressReg(uint16_t data, bool force = false);

    sitl::Connection m_connection;

    std::string m_description{};
    Timings m_writingTimings{};
    Timings m_readingTimings{};
    bool m_areBuffersEnabled = false;

    uint16_t m_serviceReg = 0x0000u;
    uint16_t m_addressReg = 0x0000u;
    bool m_isProgrammingEnabled = false;
};

} // namespace app

#endif // PROGRAMMER_SRC_MODELS_PROGRAMMER_H_
