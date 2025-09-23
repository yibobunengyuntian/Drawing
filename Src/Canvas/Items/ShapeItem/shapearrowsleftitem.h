#ifndef SHAPEARROWSLEFTITEM_H
#define SHAPEARROWSLEFTITEM_H

#include "rectopitem.h"

// 向左箭头
class ShapeArrowsLeftItem: public RectOpItem
{
public:
    ShapeArrowsLeftItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPEARROWSLEFTITEM_H
