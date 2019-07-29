/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "LinkButton.h"

#include <QApplication>

namespace app
{
LinkButton::LinkButton(QWidget *parent)
    : QLabel{parent}
{
    auto currentPalette = palette();
    currentPalette.setColor(foregroundRole(), Qt::blue);
    setPalette(currentPalette);
}


void LinkButton::mousePressEvent(QMouseEvent * /*event*/)
{
    emit clicked();
}


void LinkButton::enterEvent(QEvent * /*event*/)
{
    auto currentFont = font();
    currentFont.setUnderline(true);
    setFont(currentFont);

    QApplication::setOverrideCursor(QCursor{Qt::PointingHandCursor});
}


void LinkButton::leaveEvent(QEvent * /*event*/)
{
    auto currentFont = font();
    currentFont.setUnderline(false);
    setFont(currentFont);

    QApplication::restoreOverrideCursor();
}

} // namespace app
