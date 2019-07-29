#ifndef PROGRAMMER_SECTORSTABLEMODEL_H
#define PROGRAMMER_SECTORSTABLEMODEL_H

#include <QAbstractTableModel>

namespace app
{
struct Sector
{
    size_t size;
    bool selected;
};

class SectorsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SectorsTableModel(QObject *parent = nullptr);

    void selectAll();
    void deselectAll();

    void setItemsSelected(const std::vector<int> &items, bool selected);

private:
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QList<Sector> m_sectors{};
};

} // namespace app

#endif // PROGRAMMER_SECTORSTABLEMODEL_H
