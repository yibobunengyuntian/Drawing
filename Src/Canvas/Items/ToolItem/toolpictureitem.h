#ifndef TOOLPICTUREITEM_H
#define TOOLPICTUREITEM_H

#include "rectopitem.h"
#include <QPixmap>

// 图片
class ToolPictureItem : public RectOpItem
{
public:
    ToolPictureItem(const QPoint &startPoint, const QPoint &endPoint, const QPixmap &pixmap = QPixmap());

    virtual void paint(QPainter *painter, bool isShowOP = false) override;

    void setPicture(const QPixmap &pixmap);


protected:
    virtual void updatePath() override;

private:
    QPixmap m_pixmap;
};

#endif // TOOLPICTUREITEM_H
