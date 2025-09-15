#ifndef SHAPERIGHTTRIANGLEITEM_H
#define SHAPERIGHTTRIANGLEITEM_H

#include "shaperectopitem.h"

class ShapeRightTriangleItem: public ShapeRectOpItem
{
public:
    ShapeRightTriangleItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};
#endif // SHAPERIGHTTRIANGLEITEM_H
