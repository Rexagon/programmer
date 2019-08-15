#ifndef PROGRAMMER_SRC_OPERATIONS_PROGRAM_H_
#define PROGRAMMER_SRC_OPERATIONS_PROGRAM_H_

#include "Models/SectorTableModel.h"
#include "Operation.h"

namespace app
{
class Program final : public Operation
{
    Q_OBJECT

public:
    explicit Program(Programmer *programmer, SectorTableModel *model, const QString &fileName);

    std::optional<QString> validate() override;

    void run() override;

    QString getDescription() const override;

private:
    QString m_fileName;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_PROGRAM_H_
