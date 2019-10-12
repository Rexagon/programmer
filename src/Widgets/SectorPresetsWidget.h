#ifndef PROGRAMMER_SECTORPRESETSWIDGET_H
#define PROGRAMMER_SECTORPRESETSWIDGET_H

#include <QCheckBox>
#include <QWidget>

#include <vector>

#include "Models/SectorPresetsModel.h"

namespace app
{
/**
 * @brief   Виджет для упрощённого выбора секторов
 */
class SectorPresetsWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @param model     Таблица секторов
     * @param parent    Родительский виджет
     */
    explicit SectorPresetsWidget(SectorPresetsModel &model, QWidget *parent);

public:
    SectorPresetsWidget(const SectorPresetsWidget &) = delete;
    SectorPresetsWidget &operator=(const SectorPresetsWidget &) = delete;
    SectorPresetsWidget(SectorPresetsWidget &&) noexcept = delete;
    SectorPresetsWidget &operator=(SectorPresetsWidget &&) noexcept = delete;

private:
    void createContent();

    SectorPresetsModel &m_model;
};

} // namespace app

#endif // PROGRAMMER_SECTORPRESETSWIDGET_H
