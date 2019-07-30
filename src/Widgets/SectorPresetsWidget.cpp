/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "SectorPresetsWidget.h"

#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>

namespace
{
Qt::CheckState calculateCheckState(const std::vector<bool> &state)
{
    if (state.empty())
    {
        return Qt::CheckState::Unchecked;
    }

    bool allSelected = true;
    bool noneSelected = true;

    for (const auto &item : state)
    {
        if (allSelected && !item)
        {
            allSelected = false;
        }
        if (noneSelected && item)
        {
            noneSelected = false;
        }
    }

    if (noneSelected)
    {
        return Qt::CheckState::Unchecked;
    }
    if (allSelected)
    {
        return Qt::CheckState::Checked;
    }

    return Qt::CheckState::PartiallyChecked;
}

} // namespace


namespace app
{
SectorPresetsWidget::SectorPresetsWidget(QWidget *parent)
    : QWidget{parent}
{
    createUI();
}


void SectorPresetsWidget::setModel(app::SectorTableModel *model)
{
    assert(m_model == nullptr);

    m_model = model;

    for (auto &preset : m_presets)
    {
        connect(preset.checkBox, &QCheckBox::stateChanged, [this, &preset](int state) {
            switch (state)
            {
                case Qt::CheckState::Checked:
                    m_model->setItemsSelected(preset.sectors, true);
                    break;

                case Qt::CheckState::Unchecked:
                    m_model->setItemsSelected(preset.sectors, false);
                    preset.checkBox->setTristate(false);
                    break;

                default:
                    break;
            }
        });

        connect(m_model, &QAbstractItemModel::dataChanged, [this, &preset]() {
            const auto state = m_model->getItemsState(preset.sectors);
            preset.checkBox->setCheckState(calculateCheckState(state));
        });
    }
}


void SectorPresetsWidget::createUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *label = new QLabel("Копии ПО:", this);
    layout->addWidget(label);

    //
    std::vector<std::pair<QString, std::vector<int>>> copies = {{"Копия 1 (Сектора SA0, SA1, SA2, SA3)", {0, 1, 2, 3}},
                                                                {"Копия 2 (Сектор SA11)", {11}}};

    for (const auto &copy : copies)
    {
        auto *checkBox = new QCheckBox(copy.first, this);
        layout->addWidget(checkBox);

        m_presets.emplace_back(Preset{checkBox, copy.second});
    }
}

} // namespace app
