#ifndef PROGRAMMER_SRC_OPERATIONS_DUMP_HPP_
#define PROGRAMMER_SRC_OPERATIONS_DUMP_HPP_

#include "Operation.h"

#include <QFile>

namespace app
{
/**
 * @brief   Операция считывания текущей прошивки
 */
class Dump : public Operation
{
public:
    /**
     * @param programmer    Объект подключения к программатору
     * @param model         Пресеты секторов
     * @param fileName      Путь к файлу, в который будет записан результат
     */
    explicit Dump(Programmer &programmer, const SectorPresetsModel &model, const QString &fileName);

    /**
     * @brief   Проверяет, что выбран хотя бы один сектор
     *          и что файл для записи может быть открыт
     *
     * @return  std::nullopt, если не было ошибки
     *          QString с текстом ошибки, если она была
     */
    std::optional<QString> validate() override;

    /**
     * @brief   Считывает память в файл. Не выбранные секторы в файле
     *          будут заполнены "1"
     */
    void run() override;

    /**
     * @return  Подробное описание с указанием секторов для выгрузки
     */
    QString getDescription() const override;

private:
    void dumpPreset(const SectorPresetsModel::Preset &preset, size_t index);

    QString m_fileName;
    std::vector<std::unique_ptr<QFile>> m_files;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_DUMP_HPP_
