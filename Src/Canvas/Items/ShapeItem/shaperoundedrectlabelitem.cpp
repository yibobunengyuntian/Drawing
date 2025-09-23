#include "shaperoundedrectlabelitem.h"

ShapeRoundedRectLabelItem::ShapeRoundedRectLabelItem(const QPoint &startPoint, const QPoint &endPoint)
    :RectOpItem(startPoint, endPoint)
{
    drawing(startPoint, endPoint);
}

void ShapeRoundedRectLabelItem::updatePath()
{
    m_path.clear();

    int left = qMin(m_opPoints.first().x(), m_opPoints.last().x());
    int right = qMax(m_opPoints.first().x(), m_opPoints.last().x()) + 1;
    int top = qMin(m_opPoints.first().y(), m_opPoints.last().y());
    int bottom = qMax(m_opPoints.first().y(), m_opPoints.last().y()) + 1;

    int width = right - left;
    int height = bottom - top;
    int wRadius = width / 5.0;
    int hRadius = height / 5.0;
    m_path.moveTo(left + wRadius, top);
    m_path.lineTo(right - wRadius, top);
    m_path.arcTo(right - 2 * wRadius, top, 2 * wRadius, 2 * hRadius, 90, -90);
    m_path.lineTo(right, bottom - hRadius - height / 5.0);
    m_path.arcTo(right - 2 * wRadius, bottom - 2 * hRadius - height / 5.0, 2 * wRadius, 2 * hRadius, 0, -90);
    m_path.lineTo(left + width * 2 / 5.0, bottom - height / 5.0);
    m_path.lineTo(left + width * 3 / 10.0, bottom);
    m_path.lineTo(left + wRadius, bottom - height / 5.0);
    m_path.arcTo(left, bottom - 2 * hRadius - height / 5.0, 2 * wRadius, 2 * hRadius, 270, -90);
    m_path.lineTo(left, top + hRadius);
    m_path.arcTo(left, top, 2 * wRadius, 2 * hRadius, 180, -90);
    m_path.closeSubpath();

    RectOpItem::updatePath();
}
