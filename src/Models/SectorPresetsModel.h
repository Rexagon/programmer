#ifndef PROGRAMMER_SRC_MODELS_SECTORPRESETSMODEL_H_
#define PROGRAMMER_SRC_MODELS_SECTORPRESETSMODEL_H_

#include <vector>

#include <QString>

namespace app
{
class SectorPresetsModel
{
public:
    struct Sector
    {
        size_t number;
        size_t address;
        size_t size;
    };

    struct Preset
    {
        QString name;
        std::vector<Sector> sectors;
        bool selected;
    };

    explicit SectorPresetsModel();

    void setPresetSelected(size_t index, bool selected);

    const std::vector<Preset> &getPresets() const;
    std::vector<std::pair<Preset, size_t>> getSelectedPresets() const;

private:
    std::vector<Preset> m_presets;

};

} //

#endif // PROGRAMMER_SRC_MODELS_SECTORPRESETSMODEL_H_
