#ifndef PROGRAMMER_BAUDRATESELECTOR_H
#define PROGRAMMER_BAUDRATESELECTOR_H

#include <QComboBox>

namespace app
{
class BaudRateSelector : public QComboBox
{
    Q_OBJECT

public:
    explicit BaudRateSelector(QWidget *parent = nullptr);

private:
};

} // namespace app

#endif // PROGRAMMER_BAUDRATESELECTOR_H
