#ifndef SHAPERECTOPITEM_H
#define SHAPERECTOPITEM_H

#include "shapebaseitem.h"

class ShapeRectOpItem: public ShapeBaseItem
{
public:
    ShapeRectOpItem(const QPoint &startPoint, const QPoint &endPoint);

    virtual void drawing(const QPoint &startPoint, const QPoint &endPoint) override;
    virtual Qt::CursorShape hoverStyle(const QPoint &pos) override;

protected:
    virtual void updatePath() override;
    virtual void moveOneOp(const QPoint &pos) override;
};

#endif // SHAPERECTOPITEM_H
