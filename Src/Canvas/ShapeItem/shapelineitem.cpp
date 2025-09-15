#include "shapelineitem.h"

ShapeLineItem::ShapeLineItem(const QPoint &startPoint, const QPoint &endPoint)
    :ShapeBaseItem(startPoint, endPoint)
{
    m_opPoints.fill(QPoint(), 2);
    drawing(startPoint, endPoint);
}

void ShapeLineItem::drawing(const QPoint &startPoint, const QPoint &endPoint)
{
    m_pos = (startPoint + endPoint) / 2;
    m_opPoints[0] = startPoint - m_pos;
    m_opPoints[1] = endPoint - m_pos;

    updatePath();
}

Qt::CursorShape ShapeLineItem::hoverStyle(const QPoint &pos)
{
    int index = -1;
    for(int i = 0; i < m_opPoints.count(); ++i)
    {
        if(QLineF(m_opPoints[i], pos - m_pos).length() < 6)
        {
            index = i;
            break;
        }
    }

    if(index != -1)
    {
        return Qt::SizeVerCursor;
    }
    return Qt::CrossCursor;
}

void ShapeLineItem::updatePath()
{
    m_path.clear();
    m_path.moveTo(m_opPoints.first());
    m_path.lineTo(m_opPoints.last());
    // m_opPath = m_path;
}

void ShapeLineItem::moveOneOp(const QPoint &pos)
{
    QPoint startPos = m_opPoints.first();
    QPoint endPos = m_opPoints.last();

    switch (m_selectOpIndex) {
    case 0:
        startPos += pos;
        break;
    case 1:
        endPos += pos;
        break;
    default:
        break;
    }

    drawing(startPos + m_pos, endPos + m_pos);
}
