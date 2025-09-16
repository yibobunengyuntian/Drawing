#include "shapebaseitem.h"

ShapeBaseItem::ShapeBaseItem(const QPoint &startPoint, const QPoint &endPoint)
{
    m_pos = (startPoint + endPoint) / 2;
}

bool ShapeBaseItem::selectedOpPoint(const QPoint &pos)
{
    m_pressPos = pos - m_pos;
    for(int i = 0; i < m_opPoints.count(); ++i)
    {
        if(QLineF(m_opPoints[i], m_pressPos).length() < 6)
        {
            m_selectOpIndex = i;
            return true;
        }
    }
    m_selectOpIndex = -1;
    return m_opPath.contains(m_pressPos);
}

void ShapeBaseItem::move(const QPoint &pos)
{
    if(m_selectOpIndex != -1)
    {
        moveOneOp(pos);
    }
    else
    {
        drawing(m_opPoints.first() + m_pos + pos, m_opPoints.last() + m_pos + pos);
    }
    updatePath();
}

Qt::CursorShape ShapeBaseItem::hoverStyle(const QPoint &pos)
{
    Q_UNUSED(pos)

    return Qt::CrossCursor;
}

void ShapeBaseItem::paint(QPainter *painter, bool isShowOP)
{
    painter->save();
    painter->translate(m_pos);
    painter->drawPath(m_path);

    if(isShowOP)
    {
        QPen pen(Qt::black);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawPath(m_opPath);
        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);
        foreach (QPoint opPoint, m_opPoints) {
            painter->drawRect(opPoint.x() - 5, opPoint.y() - 5, 10, 10);
        }
    }
    painter->restore();
}

QRect ShapeBaseItem::boundingRect()
{
    QRectF rect = m_path.boundingRect();
    rect.translate(m_pos);
    return rect.toRect();
}
