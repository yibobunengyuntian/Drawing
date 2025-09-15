#include "shaperhombusitem.h"

ShapeRhombusItem::ShapeRhombusItem(const QPoint &startPoint, const QPoint &endPoint)
    :ShapeRectOpItem(startPoint, endPoint)
{
    drawing(startPoint, endPoint);
}

void ShapeRhombusItem::updatePath()
{
    m_path.clear();
    m_path.moveTo(QPointF((m_opPoints.first().x() + m_opPoints.last().x()) / 2, m_opPoints.first().y()));
    m_path.lineTo(QPointF(m_opPoints.first().x(), (m_opPoints.first().y() + m_opPoints.last().y()) / 2));
    m_path.lineTo(QPointF((m_opPoints.first().x() + m_opPoints.last().x()) / 2, m_opPoints.last().y()));
    m_path.lineTo(QPointF(m_opPoints.last().x(), (m_opPoints.first().y() + m_opPoints.last().y()) / 2));
    m_path.lineTo(QPointF((m_opPoints.first().x() + m_opPoints.last().x()) / 2, m_opPoints.first().y()));

    ShapeRectOpItem::updatePath();
}
