#include "shaperectitem.h"

ShapeRectItem::ShapeRectItem(const QPoint &startPoint, const QPoint &endPoint)
    :RectOpItem(startPoint, endPoint)
{
    drawing(startPoint, endPoint);
}

void ShapeRectItem::updatePath()
{
    m_path.clear();
    m_path.addRect(QRect(m_opPoints.first(), m_opPoints.last()));

    RectOpItem::updatePath();
}
