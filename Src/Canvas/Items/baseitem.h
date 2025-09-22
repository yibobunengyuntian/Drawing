#ifndef BASEITEM_H
#define BASEITEM_H

#include <QPainter>
#include <QPainterPath>
#include <QLine>

class BaseItem
{
public:
    BaseItem(const QPoint &startPoint, const QPoint &endPoint);

    bool selectedOpPoint(const QPoint &pos);

    virtual void move(const QPoint &pos);
    virtual Qt::CursorShape hoverStyle(const QPoint &pos);

    virtual void drawing(const QPoint &startPoint, const QPoint &endPoint) = 0;

    virtual void paint(QPainter *painter, bool isShowOP = false);

    virtual QRect boundingRect();

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

#endif // BASEITEM_H
