#ifndef SHAPEROUNDEDRECTLABELITEM_H
#define SHAPEROUNDEDRECTLABELITEM_H

#include "rectopitem.h"

// 圆角矩形标注
class ShapeRoundedRectLabelItem: public RectOpItem
{
public:
    ShapeRoundedRectLabelItem(const QPoint &startPoint, const QPoint &endPoint);

protected:
    virtual void updatePath() override;
};

#endif // SHAPEROUNDEDRECTLABELITEM_H
