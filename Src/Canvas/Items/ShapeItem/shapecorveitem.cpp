#include "shapecorveitem.h"

ShapeCorveItem::ShapeCorveItem(const QPoint &startPoint, const QPoint &endPoint)
    :BaseItem(startPoint, endPoint)
{
    m_path.moveTo(startPoint - m_pos);
    m_path.lineTo(endPoint - m_pos);
}

void ShapeCorveItem::drawing(const QPoint &startPoint, const QPoint &endPoint)
{
    m_path.lineTo(endPoint - m_pos);

    updatePath();
}

Qt::CursorShape ShapeCorveItem::hoverStyle(const QPoint &pos)
{

    return Qt::CrossCursor;
}

void ShapeCorveItem::move(const QPoint &pos)
{
    m_path.translate(pos);
    updatePath();
}

void ShapeCorveItem::updatePath()
{
    m_opPath.clear();
    // m_opPath.addRect(m_path.boundingRect());
}

void ShapeCorveItem::moveOneOp(const QPoint &pos)
{

}
