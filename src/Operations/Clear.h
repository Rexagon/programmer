#ifndef PROGRAMMER_SRC_OPERATIONS_CLEAR_H_
#define PROGRAMMER_SRC_OPERATIONS_CLEAR_H_

#include "Operation.h"

namespace app
{
class Clear : public Operation
{
public:
    explicit Clear(Programmer *programmer, SectorTableModel *model);

    std::optional<QString> validate() override;

    void run() override;

    QString getDescription() const override;

private:
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_CLEAR_H_
