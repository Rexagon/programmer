/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "SectorTableWidget.h"

#include <QHeaderView>
#include <QVBoxLayout>

namespace
{
constexpr auto TABLE_HEIGHT_MIN = 144;
}

namespace app
{
SectorTableWidget::SectorTableWidget(SectorTableModel &model, QWidget *parent)
    : QWidget{parent}
{
    createUI();
    setModel(model);
}


void SectorTableWidget::createUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_tableView = new QTableView(this);
    m_tableView->setSelectionMode(QTableView::NoSelection);
    m_tableView->setFocusPolicy(Qt::NoFocus);
    m_tableView->verticalHeader()->setVisible(false);
    m_tableView->setMinimumHeight(TABLE_HEIGHT_MIN);
    layout->addWidget(m_tableView);
}


void SectorTableWidget::setModel(SectorTableModel &model)
{
    m_tableView->setModel(&model);

    m_tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
}

} // namespace app
