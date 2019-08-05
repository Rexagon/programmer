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
    explicit Program(SectorTableModel *model, const QString &fileName);

    std::optional<QString> check() override;

    void run() override;

    QString getDescription() const override;

    Program(const Program &) = delete;
    Program &operator=(const Program &) = delete;
    Program(Program &&) noexcept = delete;
    Program &operator=(Program &&) noexcept = delete;

private:
    SectorTableModel *m_model = nullptr;

    QString m_fileName;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_PROGRAM_H_
