#ifndef PROGRAMMER_SECTORPRESETSWIDGET_H
#define PROGRAMMER_SECTORPRESETSWIDGET_H

#include <QCheckBox>
#include <QWidget>

#include <vector>

#include "Models/SectorTableModel.h"

namespace app
{
/**
 * @brief   Виджет для упрощённого выбора секторов
 */
class SectorPresetsWidget : public QWidget
{
    Q_OBJECT

    struct Preset
    {
        QCheckBox *checkBox;
        std::vector<int> sectors;
    };

public:
    /**
     * @param model     Таблица секторов
     * @param parent    Родительский виджет
     */
    explicit SectorPresetsWidget(SectorTableModel &model, QWidget *parent);

public:
    SectorPresetsWidget(const SectorPresetsWidget &) = delete;
    SectorPresetsWidget &operator=(const SectorPresetsWidget &) = delete;
    SectorPresetsWidget(SectorPresetsWidget &&) noexcept = delete;
    SectorPresetsWidget &operator=(SectorPresetsWidget &&) noexcept = delete;

private:
    void createUI();
    void setModel(SectorTableModel &model);

    std::vector<Preset> m_presets{};
};

} // namespace app

#endif // PROGRAMMER_SECTORPRESETSWIDGET_H
