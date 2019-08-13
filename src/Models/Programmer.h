#ifndef PROGRAMMER_SRC_MODELS_PROGRAMMER_H_
#define PROGRAMMER_SRC_MODELS_PROGRAMMER_H_

#include <sitl/Connection.h>

namespace app
{
class Programmer final
{
public:
    explicit Programmer(const std::string &port, unsigned int baudRate);
    ~Programmer();

private:
    sitl::Connection m_connection;

    std::string m_description{};
};

} // namespace app

#endif // PROGRAMMER_SRC_MODELS_PROGRAMMER_H_
