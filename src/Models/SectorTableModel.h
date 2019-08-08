#ifndef PROGRAMMER_SECTORTABLEMODEL_H
#define PROGRAMMER_SECTORTABLEMODEL_H

#include <QAbstractTableModel>

namespace app
{
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

    explicit SectorTableModel(QObject *parent = nullptr);

    void setItemsSelected(const std::vector<int> &items, bool selected);
    std::vector<Sector> getItems(const std::vector<int> &items) const;
    std::vector<Sector> getItems() const;

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
