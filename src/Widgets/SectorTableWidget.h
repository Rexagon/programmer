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
    /**
     * @param model     Таблица секторов
     * @param parent    Родительский виджет
     */
    explicit SectorTableWidget(SectorTableModel &model, QWidget *parent);

public:
    SectorTableWidget(const SectorTableWidget &) = delete;
    SectorTableWidget &operator=(const SectorTableWidget &) = delete;
    SectorTableWidget(SectorTableWidget &&) noexcept = delete;
    SectorTableWidget &operator=(SectorTableWidget &&) noexcept = delete;

private:
    void createUI();
    void setModel(SectorTableModel &model);

    QTableView *m_tableView = nullptr;
};

} // namespace app

#endif // PROGRAMMER_SECTORTABLEWIDGET_H
