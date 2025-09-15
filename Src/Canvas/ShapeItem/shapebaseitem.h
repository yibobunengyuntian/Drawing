#ifndef SHAPEBASEITEM_H
#define SHAPEBASEITEM_H

#include <QPainter>
#include <QPainterPath>
#include <QLine>

class ShapeBaseItem
{
public:
    ShapeBaseItem(const QPoint &startPoint, const QPoint &endPoint)
    {
        m_pos = (startPoint + endPoint) / 2;
    }

    bool selectedOpPoint(const QPoint &pos)
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

    virtual void move(const QPoint &pos)
    {
        if(m_selectOpIndex != -1)
        {
            moveOneOp(pos);
        }
        else
        {
            for(int i = 0; i < m_opPoints.count(); ++i)
            {
                m_opPoints[i] = m_opPoints[i] + pos;
            }
        }
        updatePath();
    }

    virtual Qt::CursorShape hoverStyle(const QPoint &pos)
    {
        return Qt::CrossCursor;
    }

    virtual void drawing(const QPoint &startPoint, const QPoint &endPoint) = 0;

    virtual void paint(QPainter *painter, bool isShowOP = false)
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

    QRect boundingRect()
    {
        QRectF rect = m_path.boundingRect();
        rect.translate(m_pos);
        return rect.toRect();
    }

protected:
    virtual void updatePath() = 0;
    virtual void moveOneOp(const QPoint &pos) = 0;

    QPoint m_pos;
    QPainterPath m_path;
    QPainterPath m_opPath;
    QVector<QPoint> m_opPoints;
    int m_selectOpIndex = -1;
    QPoint m_pressPos;
};

#endif // SHAPEBASEITEM_H
