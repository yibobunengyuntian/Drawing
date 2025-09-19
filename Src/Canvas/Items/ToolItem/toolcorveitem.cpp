#include "toolcorveitem.h"

ToolCorveItem::ToolCorveItem(const QPoint &startPoint, const QPoint &endPoint)
    :BaseItem(startPoint, endPoint)
{
    m_path.moveTo(startPoint - m_pos);
    m_path.lineTo(endPoint - m_pos);
    m_lastPos = endPoint - m_pos;
}

void ToolCorveItem::drawing(const QPoint &startPoint, const QPoint &endPoint)
{
    m_path.lineTo(endPoint - m_pos);
    // m_path.quadTo(m_lastPos, (endPoint - m_pos +m_lastPos)/2);
    m_lastPos = endPoint - m_pos;
    updatePath();
}

Qt::CursorShape ToolCorveItem::hoverStyle(const QPoint &pos)
{
    return Qt::CrossCursor;
}

void ToolCorveItem::move(const QPoint &pos)
{
    m_path.translate(pos);
    updatePath();
}

void ToolCorveItem::paint(QPainter *painter, bool isShowOP)
{
    painter->save();
    QPen pen = painter->pen();
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
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

void ToolCorveItem::updatePath()
{
    m_opPath.clear();
    // m_opPath.addRect(m_path.boundingRect());
}

void ToolCorveItem::moveOneOp(const QPoint &pos)
{

}
