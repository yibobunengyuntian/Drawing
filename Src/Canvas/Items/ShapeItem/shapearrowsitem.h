#ifndef SHAPEARROWSITEM_H
#define SHAPEARROWSITEM_H

#include "baseitem.h"
#include <QVector2D>

// 箭头
class ShapeArrowsItem: public BaseItem
{
public:
    ShapeArrowsItem(const QPoint &startPoint, const QPoint &endPoint);

    virtual void drawing(const QPoint &startPoint, const QPoint &endPoint) override;
    virtual Qt::CursorShape hoverStyle(const QPoint &pos) override;

    virtual void paint(QPainter *painter, bool isShowOP = false) override;

    virtual QRect boundingRect() override;

protected:
    virtual void updatePath() override;
    virtual void moveOneOp(const QPoint &pos) override;

private:
    QRect m_rect;
};

#endif // SHAPEARROWSITEM_H
