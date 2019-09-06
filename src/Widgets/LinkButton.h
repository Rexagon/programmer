#ifndef PROGRAMMER_LINKBUTTON_H
#define PROGRAMMER_LINKBUTTON_H

#include <QLabel>

namespace app
{
/**
 * @brief   Кликабельная ссылка
 */
class LinkButton final : public QLabel
{
    Q_OBJECT

public:
    /**
     * @param parent    Родительский виджет
     */
    explicit LinkButton(QWidget *parent);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

} // namespace app

#endif // PROGRAMMER_LINKBUTTON_H
