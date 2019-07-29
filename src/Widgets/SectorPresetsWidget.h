#ifndef PROGRAMMER_SECTORPRESETSWIDGET_H
#define PROGRAMMER_SECTORPRESETSWIDGET_H

#include <QCheckBox>
#include <QWidget>

#include <vector>

namespace app
{
class SectorPresetsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SectorPresetsWidget(QWidget *parent = nullptr);

    SectorPresetsWidget(const SectorPresetsWidget &) = delete;
    SectorPresetsWidget &operator=(const SectorPresetsWidget &) = delete;
    SectorPresetsWidget(SectorPresetsWidget &&) noexcept = delete;
    SectorPresetsWidget &operator=(SectorPresetsWidget &&) noexcept = delete;

private:
    void createUI();

    std::vector<QCheckBox *> m_copies{};
};

} // namespace app

#endif // PROGRAMMER_SECTORPRESETSWIDGET_H
