#ifndef SHAPELINEITEM_H
#define SHAPELINEITEM_H

#include "baseitem.h"

// 直线
class ShapeLineItem: public BaseItem
{
public:
    ShapeLineItem(const QPoint &startPoint, const QPoint &endPoint);

    virtual void drawing(const QPoint &startPoint, const QPoint &endPoint) override;
    virtual Qt::CursorShape hoverStyle(const QPoint &pos) override;

protected:
    virtual void updatePath() override;
    virtual void moveOneOp(const QPoint &pos) override;
};


#endif // SHAPELINEITEM_H
