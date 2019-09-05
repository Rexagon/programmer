#ifndef PROGRAMMER_SRC_OPERATIONS_VERIFY_H_
#define PROGRAMMER_SRC_OPERATIONS_VERIFY_H_

#include <QFile>

#include "Operation.h"

namespace app
{
class Verify final : public Operation
{
    Q_OBJECT

public:
    explicit Verify(Programmer *programmer, SectorTableModel *model, const QString &fileName);

    std::optional<QString> validate() override;

    void run() override;

    QString getDescription() const override;

private:
    QFile m_file;
    std::pair<size_t, size_t> m_range = {0, 0};
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_VERIFY_H_
