#ifndef PROGRAMMER_SRC_OPERATIONS_OPERATION_H_
#define PROGRAMMER_SRC_OPERATIONS_OPERATION_H_

#include <functional>

#include <QObject>
#include <QString>

namespace app
{
class Operation : public QObject
{
    Q_OBJECT

public:
    explicit Operation(const QString &name);

    virtual std::optional<QString> validate() = 0;

    virtual void run() = 0;

    const QString &getName() const;

    virtual QString getDescription() const = 0;

signals:
    void notifyProgress(int progress, const QString &description = "");
    void notifyComplete(bool success = true);

private:
    QString m_name;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_OPERATION_H_
