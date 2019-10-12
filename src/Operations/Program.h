#ifndef PROGRAMMER_SRC_OPERATIONS_PROGRAM_H_
#define PROGRAMMER_SRC_OPERATIONS_PROGRAM_H_

#include "Operation.h"

#include <QFile>

namespace app
{
/**
 * @brief   Операция программирования
 */
class Program final : public Operation
{
    Q_OBJECT

public:
    /**
     * @param programmer    Объект подключения к программатору
     * @param model         Пресеты секторов
     * @param fileName      Путь к файлу с прошивкой
     */
    explicit Program(Programmer &programmer, const SectorPresetsModel &model, const QString &fileName);

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
     * @brief   Записывает прошивку в выбранные сектора
     */
    void run() override;

    /**
     * @return  Подробное описание с указанием секторов для записи
     */
    QString getDescription() const override;

private:
    void programPreset(const SectorPresetsModel::Preset &preset, const QByteArray &data);

    QFile m_file;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_PROGRAM_H_
