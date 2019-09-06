#ifndef PROGRAMMER_SERIALPORTLISTMODEL_H
#define PROGRAMMER_SERIALPORTLISTMODEL_H

#include <QAbstractListModel>
#include <QtSerialPort/QSerialPortInfo>

Q_DECLARE_METATYPE(QSerialPortInfo)

namespace app
{
/**
 * @brief   Модель данных для списка COM портов
 */
class SerialPortListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    /**
     * @brief           Создаёт пустой список портов
     *
     * @param parent    Родительский QObject
     */
    explicit SerialPortListModel(QObject *parent = nullptr);

    /**
     * @brief   Заполняет список COM портами, видимыми системой
     *          в данный момент
     */
    void refresh();

    /**
     * @return  true, если список пуст
     */
    bool isEmpty() const;

protected:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    QList<QSerialPortInfo> m_serialPorts{};
};

} // namespace app

#endif // PROGRAMMER_SERIALPORTLISTMODEL_H
