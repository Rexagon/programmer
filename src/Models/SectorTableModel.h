#ifndef PROGRAMMER_SECTORTABLEMODEL_H
#define PROGRAMMER_SECTORTABLEMODEL_H

#include <QAbstractTableModel>

namespace app
{
/**
 * @brief   Модель данных для таблицы секторов
 */
class SectorTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        NAME = 0,
        SIZE = 1,
        ADDRESS = 2,
        CHECKBOX = 3,

        COUNT,

        LAST = CHECKBOX,
    };

    struct Sector
    {
        size_t number;
        size_t size;
        size_t address;
        bool selected;
    };

    /**
     *
     * @brief Создаёт таблицу из 19 секторов
     *
     * @param parent    Родительский QObject
     */
    explicit SectorTableModel(QObject *parent = nullptr);

    /**
     * @brief           Отмечает секторы с указанными индексами
     * @param items     Индексы секторов
     * @param selected  true, если помечать секторы как выбранные
     *                  false, если помечать секторы как не выбранные
     */
    void setItemsSelected(const std::vector<int> &items, bool selected);

    /**
     * @brief           Получает секторы из списка идексов
     * @param items     Индексы секоторов
     * @return          Секторы в том же порядке, что и переданные индексы
     *                  этих секторов
     */
    std::vector<Sector> getItems(const std::vector<int> &items) const;

    /**
     * @return  Все секторы
     */
    std::vector<Sector> getItems() const;

    /**
     * @return  Только выбранные сектора
     */
    std::vector<Sector> getSelectedItems() const;

protected:
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
    QList<Sector> m_sectors{};
};

} // namespace app

#endif // PROGRAMMER_SECTORTABLEMODEL_H
