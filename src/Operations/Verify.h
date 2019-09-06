#ifndef PROGRAMMER_SRC_OPERATIONS_VERIFY_H_
#define PROGRAMMER_SRC_OPERATIONS_VERIFY_H_

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

public:
    /**
     * @param programmer    Объект подключения к программатору
     * @param model         Таблица секторов
     * @param fileName      Путь к файлу с прошивкой
     */
    explicit Verify(Programmer *programmer, SectorTableModel *model, const QString &fileName);

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
    QFile m_file;
    std::pair<size_t, size_t> m_range = {0, 0};
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_VERIFY_H_
