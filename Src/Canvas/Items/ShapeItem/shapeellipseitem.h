#ifndef SHAPEELLIPSEITEM_H
#define SHAPEELLIPSEITEM_H

#include "rectopitem.h"

// 椭圆
class ShapeEllipseItem: public RectOpItem
{
public:
    ShapeEllipseItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPEELLIPSEITEM_H
