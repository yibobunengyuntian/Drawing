#ifndef SHAPEARROWSUPITEM_H
#define SHAPEARROWSUPITEM_H

#include "rectopitem.h"

// 向上箭头
class ShapeArrowsUpItem: public RectOpItem
{
public:
    ShapeArrowsUpItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPEARROWSUPITEM_H
