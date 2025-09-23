#ifndef SHAPETRIANGLEITEM_H
#define SHAPETRIANGLEITEM_H

#include "rectopitem.h"

// 三角形
class ShapeTriangleItem: public RectOpItem
{
public:
    ShapeTriangleItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};


#endif // SHAPETRIANGLEITEM_H
