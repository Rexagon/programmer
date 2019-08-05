#ifndef PROGRAMMER_SRC_OPERATIONS_OPERATION_H_
#define PROGRAMMER_SRC_OPERATIONS_OPERATION_H_

#include <functional>

#include <QObject>
#include <QString>

#include "Models/SectorTableModel.h"

namespace app
{
class Operation : public QObject
{
    Q_OBJECT

public:
    explicit Operation(SectorTableModel *model, const QString &name);

    virtual std::optional<QString> validate() = 0;

    virtual void run() = 0;

    const QString &getName() const;

    virtual QString getDescription() const = 0;

    Operation(const Operation &) = delete;
    Operation &operator=(const Operation &) = delete;
    Operation(Operation &&) noexcept = delete;
    Operation &operator=(Operation &&) noexcept = delete;

signals:
    void notifyProgress(int progress, const QString &description = "");
    void notifyComplete(bool success = true);

protected:
    SectorTableModel *getModel() const;

private:
    SectorTableModel *m_model = nullptr;

    QString m_name;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_OPERATION_H_
