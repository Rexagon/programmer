#ifndef PROGRAMMER_SECTORSTABLEWIDGET_H
#define PROGRAMMER_SECTORSTABLEWIDGET_H

#include <QTableView>
#include <QWidget>

#include "Models/SectorsTableModel.h"

namespace app
{
class SectorsTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SectorsTableWidget(QWidget *parent);

    void setModel(SectorsTableModel *model);

    SectorsTableWidget(const SectorsTableWidget &) = delete;
    SectorsTableWidget &operator=(const SectorsTableWidget &) = delete;
    SectorsTableWidget(SectorsTableWidget &&) noexcept = delete;
    SectorsTableWidget &operator=(SectorsTableWidget &&) noexcept = delete;

private:
    void createUI();

    QTableView *m_tableView = nullptr;
};

} // namespace app

#endif // PROGRAMMER_SECTORSTABLEWIDGET_H
