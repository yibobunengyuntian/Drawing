#include "shapearrowsleftitem.h"

ShapeArrowsLeftItem::ShapeArrowsLeftItem(const QPoint &startPoint, const QPoint &endPoint)
    :RectOpItem(startPoint, endPoint)
{
    drawing(startPoint, endPoint);
}

void ShapeArrowsLeftItem::updatePath()
{
    m_path.clear();

    int left = qMin(m_opPoints.first().x(), m_opPoints.last().x());
    int right = qMax(m_opPoints.first().x(), m_opPoints.last().x()) + 1;
    int top = qMin(m_opPoints.first().y(), m_opPoints.last().y());
    int bottom = qMax(m_opPoints.first().y(), m_opPoints.last().y()) + 1;

    int width = right - left;
    int height = bottom - top;

    m_path.moveTo(QPointF(left + width / 2.0, top));
    m_path.lineTo(QPointF(left, top + height / 2.0));
    m_path.lineTo(QPointF(left + width / 2.0, bottom));
    m_path.lineTo(QPointF(left + width / 2.0, bottom - height / 4.0));
    m_path.lineTo(QPointF(right, bottom - height / 4.0));
    m_path.lineTo(QPointF(right, top + height / 4.0));
    m_path.lineTo(QPointF(left + width / 2.0, top + height / 4.0));

    m_path.closeSubpath();

    RectOpItem::updatePath();
}
