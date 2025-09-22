#include "shapehexagonitem.h"

ShapeHexagonItem::ShapeHexagonItem(const QPoint &startPoint, const QPoint &endPoint)
    :RectOpItem(startPoint, endPoint)
{
    drawing(startPoint, endPoint);
}

void ShapeHexagonItem::updatePath()
{
    m_path.clear();

    int left = qMin(m_opPoints.first().x(), m_opPoints.last().x());
    int right = qMax(m_opPoints.first().x(), m_opPoints.last().x()) + 1;
    int top = qMin(m_opPoints.first().y(), m_opPoints.last().y());
    int bottom = qMax(m_opPoints.first().y(), m_opPoints.last().y()) + 1;

    int width = right - left;
    int height = bottom - top;

    // 六边形的六个顶点（所有顶点都在矩形边界上）
    m_path.moveTo(QPointF(left + width / 2.0, top)); // 上边中点
    m_path.lineTo(QPointF(right, top + height / 4.0)); // 右边1/4位置
    m_path.lineTo(QPointF(right, top + height * 3 / 4.0)); // 右边3/4位置
    m_path.lineTo(QPointF(left + width / 2, bottom)); // 底边中点
    m_path.lineTo(QPointF(left, top + height * 3 / 4.0)); // 左边3/4位置
    m_path.lineTo(QPointF(left, top + height / 4.0)); // 左边1/4位置

    m_path.closeSubpath();

    RectOpItem::updatePath();
}
