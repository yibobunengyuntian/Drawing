#include "shapetriangleitem.h"

ShapeTriangleItem::ShapeTriangleItem(const QPoint &startPoint, const QPoint &endPoint)
    :RectOpItem(startPoint, endPoint)
{
    drawing(startPoint, endPoint);
}

void ShapeTriangleItem::updatePath()
{
    m_path.clear();

    int left = qMin(m_opPoints.first().x(), m_opPoints.last().x());
    int right = qMax(m_opPoints.first().x(), m_opPoints.last().x()) + 1;
    int top = qMin(m_opPoints.first().y(), m_opPoints.last().y());
    int bottom = qMax(m_opPoints.first().y(), m_opPoints.last().y()) + 1;

    int width = right - left;

    m_path.moveTo(QPointF(left + width / 2.0, top));
    m_path.lineTo(QPointF(left, bottom));
    m_path.lineTo(QPointF(right, bottom));

    m_path.closeSubpath();

    RectOpItem::updatePath();
}
