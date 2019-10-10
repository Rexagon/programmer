#ifndef PROGRAMMER_SRC_OPERATIONS_OPERATION_H_
#define PROGRAMMER_SRC_OPERATIONS_OPERATION_H_

#include <condition_variable>
#include <functional>
#include <mutex>

#include <QObject>
#include <QString>

#include "Models/Programmer.h"
#include "Models/SectorPresetsModel.h"

namespace app
{
/**
 * @brief   Базовый абстрактный класс для операций
 */
class Operation : public QObject
{
    Q_OBJECT

public:
    struct CancelledException : public std::exception
    {
        const char *what() const noexcept override
        {
            return "operation cancelled";
        }
    };

    /**
     * @param programmer    Объект подключения к программатору
     * @param model         Таблица секторов
     * @param name          Название операции
     */
    explicit Operation(Programmer &programmer, const SectorPresetsModel &model, const QString &name);

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

    /**
     * @brief   Пометить операцию, как ожидающую отмены
     */
    void requestCancellation();

    /*
     * @brief   Разблокировать ожидающий поток результатом отмены
     *
     * @note
     * Нужно вызывать только после @a requestCancellation()
     */
    void finishCancellation(bool result);

public:
    Operation(const Operation &) = delete;
    Operation &operator=(const Operation &) = delete;
    Operation(Operation &&) noexcept = delete;
    Operation &operator=(Operation &&) noexcept = delete;

signals:
    void notifyProgress(int total, int current, const QString &description = "");
    void notifyComplete(bool success = true);
    void showCancellationDialog();

protected:
    Programmer &getProgrammer() const;
    const SectorPresetsModel &getSectorPresetsModel() const;
    const std::vector<std::pair<SectorPresetsModel::Preset, size_t>> &getSelectedPresets() const;

    void checkCancelled();

private:
    QString m_name;

    Programmer &m_programmer;
    const SectorPresetsModel &m_sectorPresetsModel;
    std::vector<std::pair<SectorPresetsModel::Preset, size_t>> m_selectedPresets;

    bool m_shouldStop = false;
    std::mutex m_cancellationMutex{};
    std::condition_variable m_cancellationGuard{};
    std::optional<bool> m_cancellationResult = std::nullopt;
};

} // namespace app

#endif // PROGRAMMER_SRC_OPERATIONS_OPERATION_H_
