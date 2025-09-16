#include "colorbutton.h"

ColorButton::ColorButton(QWidget *parent)
    :QToolButton(parent)
{

}

void ColorButton::setColor(const QColor &color)
{
    m_color = color;

    update();
}

void ColorButton::mousePressEvent(QMouseEvent *event)
{
    // 弹出颜色对话框
    QColor newColor = QColorDialog::getColor(m_color, this, "选择颜色", QColorDialog::ShowAlphaChannel);
    if (newColor.isValid() && newColor != m_color) {
        setColor(newColor);
        emit colorChanged(newColor);
    }
}

void ColorButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_color);
    QRect rect = this->rect();
    painter.drawRoundedRect(rect, 4, 4);
}
