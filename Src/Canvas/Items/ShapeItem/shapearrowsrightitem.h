#ifndef SHAPEARROWSRIGHTITEM_H
#define SHAPEARROWSRIGHTITEM_H

#include "rectopitem.h"

// 向右箭头
class ShapeArrowsRightItem: public RectOpItem
{
public:
    ShapeArrowsRightItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPEARROWSRIGHTITEM_H
