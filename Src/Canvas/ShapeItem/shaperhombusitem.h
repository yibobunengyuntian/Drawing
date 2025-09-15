#ifndef SHAPERHOMBUSITEM_H
#define SHAPERHOMBUSITEM_H

#include "shaperectopitem.h"

class ShapeRhombusItem: public ShapeRectOpItem
{
public:
    ShapeRhombusItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPERHOMBUSITEM_H
