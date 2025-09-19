#include "shapetriangleitem.h"

ShapeTriangleItem::ShapeTriangleItem(const QPoint &startPoint, const QPoint &endPoint)
    :RectOpItem(startPoint, endPoint)
{
    drawing(startPoint, endPoint);
}

void ShapeTriangleItem::updatePath()
{
    m_path.clear();
    m_path.moveTo(QPointF((m_opPoints.first().x() + m_opPoints.last().x()) / 2, m_opPoints.first().y()));
    m_path.lineTo(QPointF(m_opPoints.first().x(), m_opPoints.last().y()));
    m_path.lineTo(m_opPoints.last());
    m_path.lineTo(QPointF((m_opPoints.first().x() + m_opPoints.last().x()) / 2, m_opPoints.first().y()));

    RectOpItem::updatePath();
}
