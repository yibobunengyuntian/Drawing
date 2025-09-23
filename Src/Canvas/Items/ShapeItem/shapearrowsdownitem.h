#ifndef SHAPEARROWSDOWNITEM_H
#define SHAPEARROWSDOWNITEM_H

#include "rectopitem.h"

// 向下箭头
class ShapeArrowsDownItem: public RectOpItem
{
public:
    ShapeArrowsDownItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPEARROWSDOWNITEM_H
