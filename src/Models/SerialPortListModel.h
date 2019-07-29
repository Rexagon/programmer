#ifndef PROGRAMMER_SERIALPORTLISTMODEL_H
#define PROGRAMMER_SERIALPORTLISTMODEL_H

#include <QAbstractListModel>
#include <QtSerialPort/QSerialPortInfo>

namespace app
{
class SerialPortListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SerialPortListModel(QObject *parent = nullptr);

    void refresh();

    bool isEmpty() const;

private:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QList<QSerialPortInfo> m_serialPorts{};
};

} // namespace app

#endif // PROGRAMMER_SERIALPORTLISTMODEL_H
