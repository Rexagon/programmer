#ifndef PROGRAMMER_SRC_MODELS_PROGRAMMER_H_
#define PROGRAMMER_SRC_MODELS_PROGRAMMER_H_

#include <sitl/Connection.h>
#include <sitl/commands/Mrd.h>
#include <sitl/commands/Mwr.h>

#include "SectorTableModel.h"

namespace app
{
/**
 * @brief   Класс для работы с программатором
 */
class Programmer final
{
public:
    enum Address : uint32_t
    {
        VERSION_REG = 0x00000000u,

        SERVICE_REG = 0x00080000u,
        ADDRESS_REG = 0x00080002u,

        BIVK_DATA_BEGIN = 0x00040000u,
        BIVK_DATA_END = 0x0007FFFFu
    };

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
    enum class ReadHoldTiming : uint8_t
    {
        T2 = 0b00u,
        T3 = 0b10u,
        T4 = 0b11u
    };

    enum class ReadActiveTiming : uint8_t
    {
        T4 = 0b00u,
        T5 = 0b01u,
        T6 = 0b10u,
        T7 = 0b11u
    };

    enum class ReadSetupTiming : uint8_t
    {
        T1 = 0b00u,
        T2 = 0b01u,
        T3 = 0b10u,
        T4 = 0b11u
    };

    enum class WriteHoldTiming : uint8_t
    {
        T1 = 0b00u,
        T2 = 0b01u,
        T3 = 0b10u,
        T4 = 0b11u
    };

    enum class WriteActiveTiming : uint8_t
    {
        T3 = 0b00u,
        T4 = 0b01u,
        T5 = 0b10u,
        T6 = 0b11u
    };

    enum class WriteSetupTiming : uint8_t
    {
        T1 = 0b00u,
        T2 = 0b01u,
        T3 = 0b10u,
        T4 = 0b11u
    };

    using ReadingTimings = std::tuple<ReadSetupTiming, ReadActiveTiming, ReadHoldTiming>;
    using WritingTimings = std::tuple<WriteSetupTiming, WriteActiveTiming, WriteHoldTiming>;

    void setBuffersEnabled(bool enabled);
    void setWritingTimings(WriteSetupTiming setup, WriteActiveTiming active, WriteHoldTiming hold);
    void setReadingTimings(ReadSetupTiming setup, ReadActiveTiming active, ReadHoldTiming hold);

    void applyConfiguration();

    template <typename T>
    T readData(uint32_t address);

    template <typename T>
    void writeData(uint32_t address, T data);

    void setServiceReg(uint16_t data, bool force = false);
    void setAddressReg(uint16_t data, bool force = false);

    sitl::Connection m_connection;

    std::string m_description{};
    uint64_t m_version = 0u;

    WritingTimings m_writingTimings{};
    ReadingTimings m_readingTimings{};
    bool m_areBuffersEnabled = false;

    uint16_t m_serviceReg = 0x0000u;
    uint16_t m_addressReg = 0x0000u;
    bool m_isProgrammingEnabled = false;
};


template <typename T>
T Programmer::readData(uint32_t address)
{
    static_assert(sitl::stuff::is_any_of_v<T, uint8_t, uint16_t, uint32_t, uint64_t>, "Wrong argument type");

    setAddressReg(static_cast<uint16_t>(address >> 18u));

    address &= 0x0003FFFFu; // оставляем только 18 разрядов
    return m_connection.execute<sitl::cmds::Mrd<uint32_t, T>>(BIVK_DATA_BEGIN + address);
}

template <typename T>
void Programmer::writeData(uint32_t address, const T data)
{
    static_assert(sitl::stuff::is_any_of_v<T, uint8_t, uint16_t, uint32_t, uint64_t>, "Wrong argument type");

    setAddressReg(static_cast<uint16_t>(address >> 18u));

    address &= 0x0003FFFFu; // оставляем только 18 разрядов
    m_connection.execute<sitl::cmds::Mwr<uint32_t, T>>(BIVK_DATA_BEGIN + address, data);
}

} // namespace app

#endif // PROGRAMMER_SRC_MODELS_PROGRAMMER_H_
