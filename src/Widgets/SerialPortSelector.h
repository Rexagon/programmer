#ifndef PROGRAMMER_SERIALPORTSELECTOR_H
#define PROGRAMMER_SERIALPORTSELECTOR_H

#include <QComboBox>

#include "Models/SerialPortListModel.h"

namespace app
{
/**
 * @brief   Виджет для выбора COM порта
 */
class SerialPortSelector final : public QComboBox
{
    Q_OBJECT

public:
    /**
     * @param parent    Родительский виджет
     */
    explicit SerialPortSelector(QWidget *parent);

protected:
    void showPopup() override;

private:
    void connectSignals();

    SerialPortListModel m_serialPortListModel;
};

} // namespace app

#endif // PROGRAMMER_SERIALPORTSELECTOR_H
