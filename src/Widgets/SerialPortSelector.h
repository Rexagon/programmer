#ifndef PROGRAMMER_SERIALPORTSELECTOR_H
#define PROGRAMMER_SERIALPORTSELECTOR_H

#include <QComboBox>

#include "Models/SerialPortListModel.h"

namespace app
{
class SerialPortSelector final : public QComboBox
{
    Q_OBJECT

public:
    explicit SerialPortSelector(QWidget *parent);

private:
    void showPopup() override;

    SerialPortListModel m_serialPortListModel;
};

} // namespace app

#endif // PROGRAMMER_SERIALPORTSELECTOR_H