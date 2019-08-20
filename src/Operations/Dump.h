#ifndef PROGRAMMER_SRC_OPERATIONS_DUMP_HPP_
#define PROGRAMMER_SRC_OPERATIONS_DUMP_HPP_

#include "Operation.h"

#include <QFile>

namespace app
{
class Dump : public Operation
{
public:
    explicit Dump(Programmer *programmer, SectorTableModel *model, const QString &fileName);

    std::optional<QString> validate() override;

    void run() override;

    QString getDescription() const override;

private:
    QFile m_file;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_DUMP_HPP_
