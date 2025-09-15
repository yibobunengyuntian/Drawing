#ifndef SHAPEELLIPSEITEM_H
#define SHAPEELLIPSEITEM_H

#include "shaperectopitem.h"

class ShapeEllipseItem: public ShapeRectOpItem
{
public:
    ShapeEllipseItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPEELLIPSEITEM_H
