#ifndef PROGRAMMER_SECTORTABLEWIDGET_H
#define PROGRAMMER_SECTORTABLEWIDGET_H

#include <QTableView>
#include <QWidget>

#include "Models/SectorTableModel.h"

namespace app
{
class SectorTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SectorTableWidget(QWidget *parent);

    void setModel(SectorTableModel *model);

    SectorTableWidget(const SectorTableWidget &) = delete;
    SectorTableWidget &operator=(const SectorTableWidget &) = delete;
    SectorTableWidget(SectorTableWidget &&) noexcept = delete;
    SectorTableWidget &operator=(SectorTableWidget &&) noexcept = delete;

private:
    void createUI();

    QTableView *m_tableView = nullptr;
};

} // namespace app

#endif // PROGRAMMER_SECTORTABLEWIDGET_H
