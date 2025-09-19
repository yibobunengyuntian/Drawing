#ifndef SHAPETRIANGLEITEM_H
#define SHAPETRIANGLEITEM_H

#include "rectopitem.h"

class ShapeTriangleItem: public RectOpItem
{
public:
    ShapeTriangleItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};


#endif // SHAPETRIANGLEITEM_H
