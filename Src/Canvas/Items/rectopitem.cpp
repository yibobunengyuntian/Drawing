#include "rectopitem.h"

RectOpItem::RectOpItem(const QPoint &startPoint, const QPoint &endPoint)
    :BaseItem(startPoint, endPoint)
{
    m_opPoints.fill(QPoint(), 8);
    drawing(startPoint, endPoint);
}

void RectOpItem::drawing(const QPoint &startPoint, const QPoint &endPoint)
{
    m_pos = (startPoint + endPoint) / 2;
    QPoint sp = startPoint - m_pos;
    QPoint ep = endPoint - m_pos;
    QPoint ltPoint = QPoint(qMin(sp.x(), ep.x()), qMin(sp.y(), ep.y()));
    const int w = qAbs(sp.x() - ep.x());
    const int h = qAbs(sp.y() - ep.y());
    m_opPoints[0] = ltPoint;
    m_opPoints[1] = ltPoint + QPoint(w / 2.0, 0);
    m_opPoints[2] = ltPoint + QPoint(w, 0);

    m_opPoints[3] = ltPoint + QPoint(0, h / 2.0);
    m_opPoints[4] = ltPoint + QPoint(w, h / 2.0);

    m_opPoints[5] = ltPoint + QPoint(0, h);
    m_opPoints[6] = ltPoint + QPoint(w / 2.0, h);
    m_opPoints[7] = ltPoint + QPoint(w, h);

    updatePath();
}

Qt::CursorShape RectOpItem::hoverStyle(const QPoint &pos)
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
    if(index == 0 || index == 7)
    {
        return Qt::SizeFDiagCursor;
    }
    if(index == 2 || index == 5)
    {
        return Qt::SizeBDiagCursor;
    }
    if(index == 1 || index == 6)
    {
        return Qt::SizeVerCursor;
    }
    if(index == 3 || index == 4)
    {
        return Qt::SizeHorCursor;
    }

    if(m_opPath.contains(pos - m_pos))
    {
        return Qt::SizeAllCursor;
    }
    return Qt::CrossCursor;
}

void RectOpItem::updatePath()
{
    m_opPath.clear();
    m_opPath.addRect(QRect(m_opPoints.first(), m_opPoints.last()));
}

void RectOpItem::moveOneOp(const QPoint &pos)
{
    QPoint lt = m_opPoints.first();
    QPoint rb = m_opPoints.last();

    switch (m_selectOpIndex) {
    case 0:
        lt += pos;
        if(lt.x() > rb.x() && lt.y() > rb.y())
        {
            m_selectOpIndex = 7;
        }
        else if(lt.x() > rb.x())
        {
            m_selectOpIndex = 2;
        }
        else if(lt.y() > rb.y())
        {
            m_selectOpIndex = 5;
        }
        break;
    case 1:
        lt.setY(lt.y() + pos.y());
        if(lt.y() > rb.y())
        {
            m_selectOpIndex = 6;
        }
        break;
    case 2:
        lt.setY(lt.y() + pos.y());
        rb.setX(rb.x() + pos.x());
        if(lt.x() > rb.x() && lt.y() > rb.y())
        {
            m_selectOpIndex = 5;
        }
        else if(lt.x() > rb.x())
        {
            m_selectOpIndex = 0;
        }
        else if(lt.y() > rb.y())
        {
            m_selectOpIndex = 7;
        }
        break;
    case 3:
        lt.setX(lt.x() + pos.x());
        if(lt.x() > rb.x())
        {
            m_selectOpIndex = 4;
        }
        break;
    case 4:
        rb.setX(rb.x() + pos.x());
        if(lt.x() > rb.x())
        {
            m_selectOpIndex = 3;
        }
        break;
    case 5:
        lt.setX(lt.x() + pos.x());
        rb.setY(rb.y() + pos.y());
        if(lt.x() > rb.x() && lt.y() > rb.y())
        {
            m_selectOpIndex = 2;
        }
        else if(lt.x() > rb.x())
        {
            m_selectOpIndex = 7;
        }
        else if(lt.y() > rb.y())
        {
            m_selectOpIndex = 0;
        }
        break;
    case 6:
        rb.setY(rb.y() + pos.y());
        if(lt.y() > rb.y())
        {
            m_selectOpIndex = 1;
        }
        break;
    case 7:
        rb += pos;
        if(lt.x() > rb.x() && lt.y() > rb.y())
        {
            m_selectOpIndex = 0;
        }
        else if(lt.x() > rb.x())
        {
            m_selectOpIndex = 5;
        }
        else if(lt.y() > rb.y())
        {
            m_selectOpIndex = 2;
        }
        break;
    default:
        break;
    }

    drawing(lt + m_pos, rb + m_pos);
}
