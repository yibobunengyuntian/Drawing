#ifndef RECTOPITEM_H
#define RECTOPITEM_H

#include "baseitem.h"

class RectOpItem: public BaseItem
{
public:
    RectOpItem(const QPoint &startPoint, const QPoint &endPoint);

    virtual void drawing(const QPoint &startPoint, const QPoint &endPoint) override;
    virtual Qt::CursorShape hoverStyle(const QPoint &pos) override;

protected:
    virtual void updatePath() override;
    virtual void moveOneOp(const QPoint &pos) override;
};

#endif // RECTOPITEM_H
