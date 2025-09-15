#ifndef SHAPECORVEITEM_H
#define SHAPECORVEITEM_H

#include "shapebaseitem.h"

class ShapeCorveItem: public ShapeBaseItem
{
public:
    ShapeCorveItem(const QPoint &startPoint, const QPoint &endPoint);

    virtual void drawing(const QPoint &startPoint, const QPoint &endPoint) override;
    virtual Qt::CursorShape hoverStyle(const QPoint &pos) override;
    virtual void move(const QPoint &pos) override;

protected:
    virtual void updatePath() override;
    virtual void moveOneOp(const QPoint &pos) override;
};


#endif // SHAPECORVEITEM_H
