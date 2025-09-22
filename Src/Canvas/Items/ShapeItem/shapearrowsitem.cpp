#include "shapearrowsitem.h"

ShapeArrowsItem::ShapeArrowsItem(const QPoint &startPoint, const QPoint &endPoint)
    :BaseItem(startPoint, endPoint)
{
    m_opPoints.fill(QPoint(), 2);
    drawing(startPoint, endPoint);
}

void ShapeArrowsItem::drawing(const QPoint &startPoint, const QPoint &endPoint)
{
    m_pos = (startPoint + endPoint) / 2;
    m_opPoints[0] = startPoint - m_pos;
    m_opPoints[1] = endPoint - m_pos;

    updatePath();
}

Qt::CursorShape ShapeArrowsItem::hoverStyle(const QPoint &pos)
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

void ShapeArrowsItem::paint(QPainter *painter, bool isShowOP)
{
    QPointF p1 = m_opPoints.first();
    QPointF p2 = m_opPoints.last();
    float w = qMax(painter->pen().width() * 5, 10);
    float l = qMin(QLineF(p1, p2).length() / 2, w);
    QPointF point = p2 - QVector2D(p2 - p1).normalized().toPointF()*l;
    qreal angle = qDegreesToRadians(QLineF(p1, p2).angle());    //与x轴夹角
    QPolygonF polygon;
    polygon.append(QPointF(point.x() - w / 2 * qSin(angle), point.y() - w / 2 * qCos(angle)));
    polygon.append(QPointF(point.x() + w / 2 * qSin(angle), point.y() + w / 2 * qCos(angle)));
    polygon.append(p2);

    painter->save();
    painter->translate(m_pos);
    painter->drawLine(p1, point);
    painter->setBrush(painter->pen().color());
    painter->setPen(Qt::NoPen);
    painter->drawPolygon(polygon);

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

    QRectF rect = polygon.boundingRect();
    m_rect = rect.united(QRectF(p1, point.toPoint())).toRect();
}

QRect ShapeArrowsItem::boundingRect()
{
    return m_rect.translated(m_pos);
}

void ShapeArrowsItem::updatePath()
{
    m_path.clear();
    m_path.moveTo(m_opPoints.first());
    m_path.lineTo(m_opPoints.last());
    // m_opPath = m_path;
}

void ShapeArrowsItem::moveOneOp(const QPoint &pos)
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
