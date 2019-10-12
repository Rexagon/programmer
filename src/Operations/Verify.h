#ifndef PROGRAMMER_SRC_OPERATIONS_VERIFY_H_
#define PROGRAMMER_SRC_OPERATIONS_VERIFY_H_

#include <optional>

#include <QFile>

#include "Operation.h"

namespace app
{
/**
 * @brief   Операция проверки прошивки в БИВК
 */
class Verify final : public Operation
{
    Q_OBJECT

    using Differences = std::pair<const SectorPresetsModel::Preset&, std::vector<size_t>>;

public:
    /**
     * @param programmer    Объект подключения к программатору
     * @param model         Пресеты секторов
     * @param fileName      Путь к файлу с прошивкой
     */
    explicit Verify(Programmer &programmer, const SectorPresetsModel &model, const QString &fileName);

    /**
     * @brief   Проверяет, что выбран хотя бы один сектор,
     *          непрерывны ли выбранные сектора и
     *          что можно ли открыть файл с прошивкой
     *
     * @return  std::nullopt, если не было ошибки
     *          QString с текстом ошибки, если она была
     */
    std::optional<QString> validate() override;

    /**
     * @brief   Сравнивает прошивку в выбранных секторах с
     *          той, что в файле
     */
    void run() override;

    /**
     *          для срав
     * @return  Подробное описание с указанием сравниваемых секторов
     */
    QString getDescription() const override;

private:
    std::optional<Differences> verifyPreset(const SectorPresetsModel::Preset &preset, const QByteArray &data);

    QFile m_file;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_VERIFY_H_
