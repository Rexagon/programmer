#ifndef PROGRAMMER_SRC_MODELS_PROGRAMMER_H_
#define PROGRAMMER_SRC_MODELS_PROGRAMMER_H_

#include <sitl/Connection.h>

#include "SectorTableModel.h"

namespace app
{
/**
 * @brief   Класс для работы с программатором
 */
class Programmer final
{
    using Timings = std::tuple<uint8_t, uint8_t, uint8_t>;

public:
    /**
     * @brief           Создаёт подключение к программатору
     *
     * @note
     * В один момент времени может быть установлено только одно соединение через
     * конкретный COM порт.
     *
     * @param port      Название COM порта
     * @param baudRate  Скорость
     *
     * @throws          std::runtime_error если не удалось подключится в течении 2с.
     */
    explicit Programmer(const std::string &port, unsigned int baudRate);

    /**
     * @brief           Закрывает соединение с программатором.
     *
     * Перед закрытием обнуляет сервисный и адресный регистры.
     */
    ~Programmer();

    /**
     * @brief           Подготовливает программатор к работе
     *
     * Обнуляет адресный регистр, заполняет сервисный регистр по умолчанию.
     */
    void reset();

    /**
     * @brief               Читает массив данных из БИВК
     *
     * @param data [out]    Массив, куда запишутся данные
     * @param begin         Адресс начала считывания
     * @param size          Размер считываемых данных
     *
     * @throws std::logic_error если `begin + size` выходит за пределы памяти
     */
    void readData(std::vector<uint8_t> &data, size_t begin, size_t size);

    /**
     * @brief           Записывает массив данных в программатор
     *
     * @note
     * Перед вызовом данной функции должен быть включён режим программирования
     * @a enableProgramming()
     *
     * @note
     * Бит данных не может быть изменён с "0" на "1"  помощью данной функции.
     * Для этого надо очистить затрагиваемые сектора с помощью @a clearSector()
     *
     * @param data      Массив с данными
     * @param begin     Адрес начала записи
     * @param size      Размер записываемых данных
     *
     * @throws std::runtime_error если режим программирования выключен
     * @throws std::logic_error если `begin + size` выходит за пределы памяти
     */
    void writeData(const void *data, size_t begin, size_t size);

    /**
     * @brief   Включает режим программирования
     *
     * В данном режиме запись происходит всего за 2 цикла, поэтому
     * использование этой функции обязательно перед вызовом writeData()
     *
     * @note
     * При включённом режиме программирования нельзя считывать данные
     */
    void enableProgramming();

    /**
     * @brief   Выключает режим программирования
     */
    void disableProgramming();

    /**
     * @brief           Очищает сектор, заполняя его "1"
     * @param sector    Сектор, который будет очищен
     */
    void clearSector(const SectorTableModel::Sector &sector);

    /**
     * @return  Описание подключённого программатора
     */
    const std::string &getDescription() const;

    /**
     * @return  Версия подключённого программатора
     */
    uint64_t getVersion() const;

private:
    void setBuffersEnabled(bool enabled);
    void setWritingTimings(uint8_t setup, uint8_t active, uint8_t hold);
    void setReadingTimings(uint8_t setup, uint8_t active, uint8_t hold);

    void applyConfiguration();

    uint8_t readData(uint32_t address);
    void writeData(uint32_t address, uint8_t data);
    void writeData(uint32_t address, uint32_t data);

    void setServiceReg(uint16_t data, bool force = false);
    void setAddressReg(uint16_t data, bool force = false);

    sitl::Connection m_connection;

    std::string m_description{};
    uint64_t m_version = 0u;

    Timings m_writingTimings{};
    Timings m_readingTimings{};
    bool m_areBuffersEnabled = false;

    uint16_t m_serviceReg = 0x0000u;
    uint16_t m_addressReg = 0x0000u;
    bool m_isProgrammingEnabled = false;
};

} // namespace app

#endif // PROGRAMMER_SRC_MODELS_PROGRAMMER_H_
