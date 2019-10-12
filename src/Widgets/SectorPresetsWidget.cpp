/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "SectorPresetsWidget.h"

#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>

#include "../Settings.h"

namespace app
{
SectorPresetsWidget::SectorPresetsWidget(SectorPresetsModel &model, QWidget *parent)
    : QWidget{parent}
    , m_model{model}
{
    createContent();
}


void SectorPresetsWidget::createContent()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *label = new QLabel("Копии ПО:", this);
    layout->addWidget(label);

    //
    const auto &presets = m_model.getPresets();
    for (size_t i = 0; i < presets.size(); ++i)
    {
        auto *checkBox = new QCheckBox(presets[i].name, this);
        checkBox->setTristate(false);
        layout->addWidget(checkBox);

        checkBox->setChecked(presets[i].selected);

        connect(checkBox, &QCheckBox::stateChanged, [this, i](int state) {
            m_model.setPresetSelected(i, state == Qt::CheckState::Checked);
        });
    }
}

} // namespace app
