[![pipeline status](http://git.module.ru/P13/programmer/badges/develop/pipeline.svg)](http://git.module.ru/P13/programmer/commits/develop)

# ROM Programmer

Программа для работы с памятью БИВК.

Аппаратная часть выполнена Иваном Поляковым ([i.polyakov@module.ru](mailto:i.polyakov@module.ru))
Программная часть выполнена Иваном Калининым ([i.kalinin@module.ru](mailto:i.kalinin@module.ru))

Подробнее про программатор:
`\\dragon\docum\Archive\i.kalinin\API_prog008.doc`

Общение с программатором реализовано через протокол SITL. Использовалась документация:
`\\dragon\docum\Archive\i.kalinin\SITL_1.0_2014.doc`

## Структура проекта

* `Models/Programmer.cpp` - обёртка для общения с программатором
* `main.cpp` - открывает окно `MainWindow`
* `Windows/MainWindow` - содержит в себе всё состояние (см `General.h`), связывает сигналы
* `Windows/MainWindowUI` - строит интерфейс приложения
  * В верхней части окна создаётся `ConnectionWidget`, `SectorPresetsWidget` и строка с версией
  * В нижней части окна создаются кнопки
* `Widgets/ConnectionWidget` - виджет, группирующий поля для подключения. Может находится в 3х состояниях:
  * Отключён - показывается `SerialPortSelector` и кнопка подключения
  * Подключается - неактивные поля предыдущего состояния
  * Подключён - вывод информации о подключени и кнопка отключить
* `Widgets/SerialPortSelector` - подтягивает доступные порты в системе при создании. Может запоминать выбор (см. `Settings.h`)
* `Widgets/SectorPresetsWidget.h` - показывает список различных вариантов записи (Копия 1, Копия 2, ...). Может запоминать выбор (см. `Settings.h`). Связан с моделью `Models/SectorPresetsModel.h`