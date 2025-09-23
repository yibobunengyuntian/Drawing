#ifndef SHAPERIGHTTRIANGLEITEM_H
#define SHAPERIGHTTRIANGLEITEM_H

#include "rectopitem.h"

// 直角三角形
class ShapeRightTriangleItem: public RectOpItem
{
public:
    ShapeRightTriangleItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};
#endif // SHAPERIGHTTRIANGLEITEM_H
