/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "SectorsPresetsWidget.h"

#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>

namespace app
{
SectorsPresetsWidget::SectorsPresetsWidget(QWidget *parent)
    : QWidget{parent}
{
    createUI();
}


void SectorsPresetsWidget::setModel(app::SectorsTableModel *model)
{
    if (m_model != nullptr)
    {
        return;
    }

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

            auto checkState = Qt::CheckState::PartiallyChecked;
            if (noneSelected)
            {
                checkState = Qt::CheckState::Unchecked;
            }
            if (allSelected)
            {
                checkState = Qt::CheckState::Checked;
            }

            preset.checkBox->setCheckState(checkState);
        });
    }
}


void SectorsPresetsWidget::createUI()
{
    auto *layout = new QVBoxLayout(this);

    auto *label = new QLabel("Копии ПО:", this);
    layout->addWidget(label);

    //
    std::vector<std::pair<QString, std::vector<int>>> copies = {{"Копия 1 (Сектора SA0, SA1, SA2, SA3)", {0, 1, 2, 3}},
                                                                {"Копия 2 (Сектор SA11)", {11}}};

    for (const auto &copy : copies)
    {
        auto *checkbox = new QCheckBox(copy.first, this);
        layout->addWidget(checkbox);

        m_presets.emplace_back(Preset{checkbox, copy.second});
    }
}

} // namespace app
