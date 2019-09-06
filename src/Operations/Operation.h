#ifndef PROGRAMMER_SRC_OPERATIONS_OPERATION_H_
#define PROGRAMMER_SRC_OPERATIONS_OPERATION_H_

#include <functional>

#include <QObject>
#include <QString>

#include "Models/Programmer.h"
#include "Models/SectorTableModel.h"

namespace app
{
/**
 * @brief   Базовый абстрактный класс для операций
 */
class Operation : public QObject
{
    Q_OBJECT

public:
    /**
     * @param programmer    Объект подключения к программатору
     * @param model         Таблица секторов
     * @param name          Название операции
     */
    explicit Operation(Programmer &programmer, const SectorTableModel &model, const QString &name);

    /**
     * @brief   Проверяет возможность выполнения операции
     *
     * @return  std::nullopt, если не было ошибки
     *          QString с текстом ошибки, если она была
     */
    virtual std::optional<QString> validate() = 0;

    /**
     * @brief   Запускает выполнение операции
     *
     * @note
     * Во время выполнения вызываются сигналы @a notifyProgress() и
     * @a notifyComplete(), поэтому данный метод лучше запускать в
     * отдельном потоке
     *
     * @sa notifyProgress(), notifyComplete()
     */
    virtual void run() = 0;

    /**
     * @return  Название операции
     */
    const QString &getName() const;

    /**
     * @return  Подробное описание конкретной операции
     */
    virtual QString getDescription() const = 0;

public:
    Operation(const Operation &) = delete;
    Operation &operator=(const Operation &) = delete;
    Operation(Operation &&) noexcept = delete;
    Operation &operator=(Operation &&) noexcept = delete;

signals:
    void notifyProgress(int total, int current, const QString &description = "");
    void notifyComplete(bool success = true);

protected:
    Programmer &getProgrammer() const;
    const SectorTableModel &getSectorTableModel() const;

    const std::vector<SectorTableModel::Sector> &getSelectedSectors() const;

private:
    QString m_name;

    Programmer &m_programmer;
    const SectorTableModel &m_sectorTableModel;

    std::vector<SectorTableModel::Sector> m_selectedSectors;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_OPERATION_H_
