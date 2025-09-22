#include "shaperighttriangleitem.h"

ShapeRightTriangleItem::ShapeRightTriangleItem(const QPoint &startPoint, const QPoint &endPoint)
    :RectOpItem(startPoint, endPoint)
{
    drawing(startPoint, endPoint);
}

void ShapeRightTriangleItem::updatePath()
{
    m_path.clear();

    int left = qMin(m_opPoints.first().x(), m_opPoints.last().x());
    int right = qMax(m_opPoints.first().x(), m_opPoints.last().x()) + 1;
    int top = qMin(m_opPoints.first().y(), m_opPoints.last().y());
    int bottom = qMax(m_opPoints.first().y(), m_opPoints.last().y()) + 1;

    m_path.moveTo(QPointF(left, top));
    m_path.lineTo(QPointF(left, bottom));
    m_path.lineTo(QPointF(right, bottom));

    m_path.closeSubpath();

    RectOpItem::updatePath();
}
