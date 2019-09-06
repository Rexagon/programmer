#ifndef PROGRAMMER_BAUDRATESELECTOR_H
#define PROGRAMMER_BAUDRATESELECTOR_H

#include <QComboBox>

namespace app
{
/**
 * @brief   Виджет для выбора стандартных скоростей COM порта
 */
class BaudRateSelector : public QComboBox
{
    Q_OBJECT

public:
    /**
     * @param parent    Родительский виджет
     */
    explicit BaudRateSelector(QWidget *parent);
};

} // namespace app

#endif // PROGRAMMER_BAUDRATESELECTOR_H
