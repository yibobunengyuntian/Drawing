#ifndef TOOLCORVEITEM_H
#define TOOLCORVEITEM_H

#include <QApplication>
#include <QCursor>
#include <QWidget>

#include "baseitem.h"

class ToolCorveItem: public BaseItem
{
public:
    ToolCorveItem(const QPoint &startPoint, const QPoint &endPoint);

    virtual void drawing(const QPoint &startPoint, const QPoint &endPoint) override;
    virtual Qt::CursorShape hoverStyle(const QPoint &pos) override;
    virtual void move(const QPoint &pos) override;

    virtual void paint(QPainter *painter, bool isShowOP = false) override;

protected:
    virtual void updatePath() override;
    virtual void moveOneOp(const QPoint &pos) override;

private:
    QPoint m_lastPos;
};


#endif // TOOLCORVEITEM_H
