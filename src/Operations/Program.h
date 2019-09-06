#ifndef PROGRAMMER_SRC_OPERATIONS_PROGRAM_H_
#define PROGRAMMER_SRC_OPERATIONS_PROGRAM_H_

#include "Models/SectorTableModel.h"
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
     * @param model         Таблица секотров
     * @param fileName      Путь к файлу с прошивкой
     */
    explicit Program(Programmer *programmer, SectorTableModel *model, const QString &fileName);

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
    QFile m_file;
    std::pair<size_t, size_t> m_range = {0, 0};
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_PROGRAM_H_
