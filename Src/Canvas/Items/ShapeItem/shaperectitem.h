#ifndef SHAPERECTITEM_H
#define SHAPERECTITEM_H

#include "rectopitem.h"

class ShapeRectItem: public RectOpItem
{
public:
    ShapeRectItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPERECTITEM_H
