#include "shaperighttriangleitem.h"

ShapeRightTriangleItem::ShapeRightTriangleItem(const QPoint &startPoint, const QPoint &endPoint)
    :ShapeRectOpItem(startPoint, endPoint)
{
    drawing(startPoint, endPoint);
}

void ShapeRightTriangleItem::updatePath()
{
    m_path.clear();
    m_path.moveTo(m_opPoints.first());
    m_path.lineTo(QPointF(m_opPoints.first().x(), m_opPoints.last().y()));
    m_path.lineTo(m_opPoints.last());
    m_path.lineTo(m_opPoints.first());

    ShapeRectOpItem::updatePath();
}
