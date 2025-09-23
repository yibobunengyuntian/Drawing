#ifndef SHAPEHEXAGONITEM_H
#define SHAPEHEXAGONITEM_H

#include "rectopitem.h"

// 六边形
class ShapeHexagonItem: public RectOpItem
{
public:
    ShapeHexagonItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPEHEXAGONITEM_H
