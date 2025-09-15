#ifndef SHAPETRIANGLEITEM_H
#define SHAPETRIANGLEITEM_H

#include "shaperectopitem.h"

class ShapeTriangleItem: public ShapeRectOpItem
{
public:
    ShapeTriangleItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};


#endif // SHAPETRIANGLEITEM_H
