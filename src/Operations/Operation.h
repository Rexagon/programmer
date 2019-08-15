#ifndef PROGRAMMER_SRC_OPERATIONS_OPERATION_H_
#define PROGRAMMER_SRC_OPERATIONS_OPERATION_H_

#include <functional>

#include <QObject>
#include <QString>

#include "Models/Programmer.h"
#include "Models/SectorTableModel.h"

namespace app
{
class Operation : public QObject
{
    Q_OBJECT

public:
    explicit Operation(Programmer *programmer, SectorTableModel *model, const QString &name);

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
    Programmer *getProgrammer() const;
    SectorTableModel *getSectorTableModel() const;

    const std::vector<SectorTableModel::Sector> &getSelectedSectors() const;

private:
    QString m_name;

    Programmer *m_programmer;
    SectorTableModel *m_sectorTableModel;

    std::vector<SectorTableModel::Sector> m_selectedSectors;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_OPERATION_H_
