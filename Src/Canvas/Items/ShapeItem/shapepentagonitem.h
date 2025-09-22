#ifndef SHAPEPENTAGONITEM_H
#define SHAPEPENTAGONITEM_H

#include "rectopitem.h"

class ShapePentagonItem: public RectOpItem
{
public:
    ShapePentagonItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPEPENTAGONITEM_H
