#include "toolpictureitem.h"

ToolPictureItem::ToolPictureItem(const QPoint &startPoint, const QPoint &endPoint, const QPixmap &pixmap)
    :RectOpItem(startPoint, endPoint)
{
    m_pixmap = pixmap;
    drawing(startPoint, endPoint);
}

void ToolPictureItem::paint(QPainter *painter, bool isShowOP)
{
    painter->save();
    painter->translate(m_pos);

    if(!m_pixmap.isNull())
    {
        float w = m_opPath.boundingRect().width();
        float h = m_opPath.boundingRect().height();

        painter->drawPixmap(-w / 2, -h / 2, w, h, m_pixmap);
    }


    if(isShowOP)
    {
        // 设置虚线样式
        QPen pen(Qt::black);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawPath(m_opPath);
        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);
        foreach (QPoint opPoint, m_opPoints) {
            painter->drawRect(opPoint.x() - 5, opPoint.y() - 5, 10, 10);
        }
    }
    painter->restore();
}

void ToolPictureItem::updatePath()
{
    m_path.clear();
    m_path.addRect(QRect(m_opPoints.first(), m_opPoints.last()));

    RectOpItem::updatePath();
}
