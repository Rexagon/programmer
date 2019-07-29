/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "SectorPresetsWidget.h"

#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>

namespace app
{
SectorPresetsWidget::SectorPresetsWidget(QWidget *parent)
    : QWidget{parent}
{
    createUI();
}


void SectorPresetsWidget::createUI()
{
    auto *layout = new QVBoxLayout(this);

    auto *label = new QLabel("Копии ПО:", this);
    layout->addWidget(label);

    //
    std::vector<QString> copies = {"Копия 1 (Сектора SA0, SA1, SA2, SA3)", "Копия 2 (Сектор SA11)"};

    for (const auto &copy : copies)
    {
        auto *checkbox = new QCheckBox(copy, this);
        layout->addWidget(checkbox);

        m_copies.emplace_back(checkbox);
    }
}

} // namespace app
