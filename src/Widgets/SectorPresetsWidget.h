#ifndef PROGRAMMER_SECTORPRESETSWIDGET_H
#define PROGRAMMER_SECTORPRESETSWIDGET_H

#include <QCheckBox>
#include <QWidget>

#include <vector>

#include "Models/SectorTableModel.h"

namespace app
{
class SectorPresetsWidget : public QWidget
{
    Q_OBJECT

    struct Preset
    {
        QCheckBox *checkBox;
        std::vector<int> sectors;
    };

public:
    explicit SectorPresetsWidget(QWidget *parent = nullptr);

    void setModel(SectorTableModel *model);

    SectorPresetsWidget(const SectorPresetsWidget &) = delete;
    SectorPresetsWidget &operator=(const SectorPresetsWidget &) = delete;
    SectorPresetsWidget(SectorPresetsWidget &&) noexcept = delete;
    SectorPresetsWidget &operator=(SectorPresetsWidget &&) noexcept = delete;

private:
    void createUI();

    std::vector<Preset> m_presets{};

    SectorTableModel *m_model = nullptr;
};

} // namespace app

#endif // PROGRAMMER_SECTORPRESETSWIDGET_H
