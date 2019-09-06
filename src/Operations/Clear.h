#ifndef PROGRAMMER_SRC_OPERATIONS_CLEAR_H_
#define PROGRAMMER_SRC_OPERATIONS_CLEAR_H_

#include "Operation.h"

namespace app
{
/**
 * @brief   Операция очистки секторов
 */
class Clear : public Operation
{
public:
    /**
     * @param programmer    Объект подключения к программатору
     * @param model         Таблица секторов
     */
    explicit Clear(Programmer &programmer, const SectorTableModel &model);

    /**
     * @brief   Проверяет, что выбран хотя бы один сектор
     *
     * @return  std::nullopt, если не было ошибки
     *          QString с текстом ошибки, если она была
     */
    std::optional<QString> validate() override;

    /**
     * @brief   Очищает выбранные секторы
     */
    void run() override;

    /**
     * @return  Подробное описание с указанием секторов для очистки
     */
    QString getDescription() const override;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_CLEAR_H_
