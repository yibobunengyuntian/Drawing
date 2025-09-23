#ifndef SHAPEPENTAGONITEM_H
#define SHAPEPENTAGONITEM_H

#include "rectopitem.h"

// 五边形
class ShapePentagonItem: public RectOpItem
{
public:
    ShapePentagonItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPEPENTAGONITEM_H
