#ifndef SHAPERHOMBUSITEM_H
#define SHAPERHOMBUSITEM_H

#include "rectopitem.h"

class ShapeRhombusItem: public RectOpItem
{
public:
    ShapeRhombusItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPERHOMBUSITEM_H
