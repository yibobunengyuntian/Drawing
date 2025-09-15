#ifndef SHAPERECTITEM_H
#define SHAPERECTITEM_H

#include "shaperectopitem.h"

class ShapeRectItem: public ShapeRectOpItem
{
public:
    ShapeRectItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPERECTITEM_H
