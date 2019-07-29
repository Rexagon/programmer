#ifndef PROGRAMMER_SECTORSPRESETSWIDGET_H
#define PROGRAMMER_SECTORSPRESETSWIDGET_H

#include <QCheckBox>
#include <QWidget>

#include <vector>

#include "Models/SectorsTableModel.h"

namespace app
{
class SectorsPresetsWidget : public QWidget
{
    Q_OBJECT

    struct Preset
    {
        QCheckBox *checkBox;
        std::vector<int> sectors;
    };

public:
    explicit SectorsPresetsWidget(QWidget *parent = nullptr);

    void setModel(SectorsTableModel *model);

    SectorsPresetsWidget(const SectorsPresetsWidget &) = delete;
    SectorsPresetsWidget &operator=(const SectorsPresetsWidget &) = delete;
    SectorsPresetsWidget(SectorsPresetsWidget &&) noexcept = delete;
    SectorsPresetsWidget &operator=(SectorsPresetsWidget &&) noexcept = delete;

private:
    void createUI();

    std::vector<Preset> m_presets{};

    SectorsTableModel *m_model = nullptr;
};

} // namespace app

#endif // PROGRAMMER_SECTORSPRESETSWIDGET_H
