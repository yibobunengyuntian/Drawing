#include "shapeellipseitem.h"

ShapeEllipseItem::ShapeEllipseItem(const QPoint &startPoint, const QPoint &endPoint)
    :RectOpItem(startPoint, endPoint)
{
    drawing(startPoint, endPoint);
}

void ShapeEllipseItem::updatePath()
{
    m_path.clear();
    m_path.addEllipse(QRect(m_opPoints.first(), m_opPoints.last()));

    RectOpItem::updatePath();
}
