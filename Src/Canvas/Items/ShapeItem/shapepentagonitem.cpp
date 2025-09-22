#include "shapepentagonitem.h"

ShapePentagonItem::ShapePentagonItem(const QPoint &startPoint, const QPoint &endPoint)
    :RectOpItem(startPoint, endPoint)
{
    drawing(startPoint, endPoint);
}

void ShapePentagonItem::updatePath()
{
    m_path.clear();

    // 获取矩形边界
    int left = qMin(m_opPoints.first().x(), m_opPoints.last().x());
    int right = qMax(m_opPoints.first().x(), m_opPoints.last().x()) + 1;
    int top = qMin(m_opPoints.first().y(), m_opPoints.last().y());
    int bottom = qMax(m_opPoints.first().y(), m_opPoints.last().y()) + 1;

    int width = right - left;
    int height = bottom - top;

    m_path.moveTo(QPointF(left + width / 2.0, top)); // 顶部中点

    m_path.lineTo(QPointF(right, top + height * 0.35)); // 右上侧
    m_path.lineTo(QPointF(right - width * 0.15, bottom)); // 右下侧
    m_path.lineTo(QPointF(left + width * 0.15, bottom)); // 左下侧
    m_path.lineTo(QPointF(left, top + height * 0.35)); // 左上侧

    m_path.closeSubpath();

    RectOpItem::updatePath();
}
