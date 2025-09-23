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
    Q_UNUSED(event)
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

    painter.setPen(Qt::black);
    painter.setBrush(m_color);
    QRect rect = this->rect();
    painter.drawPixmap(rect, m_gridPixmap);
    painter.drawRoundedRect(rect, 4, 4);
}

void ColorButton::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    m_gridPixmap = QPixmap(this->size());
    QPainter painter(&m_gridPixmap);;
    int gridSize = 5; // 每个格子的大小
    int w = this->width();
    int h = this->height();
    for (int y = 0; y < h; y += gridSize) {
        for (int x = 0; x < w; x += gridSize) {
            // 判断当前格子位置，决定颜色
            bool isLight = ((x / gridSize) % 2) ^ ((y / gridSize) % 2);
            QColor color = isLight ? QColor(240, 240, 240) : QColor(200, 200, 200);

            painter.fillRect(x, y, gridSize, gridSize, color);
        }
    }
}
