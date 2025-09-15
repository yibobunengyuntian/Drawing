#ifndef SHAPETEXTITEM_H
#define SHAPETEXTITEM_H

#include "shapebaseitem.h"
#include <QFont>
#include <QString>

class ShapeTextItem : public ShapeBaseItem
{
public:
    ShapeTextItem(const QPoint &startPoint, const QPoint &endPoint);


    void setText(const QString &text);

    QString text() const;

    void setFont(const QFont &font);

    QFont font() const;

    virtual void drawing(const QPoint &startPoint, const QPoint &endPoint) override;

    virtual void paint(QPainter *painter, bool isShowOP = false) override;


protected:
    virtual void updatePath() override;

    virtual void moveOneOp(const QPoint &pos) override;

    virtual Qt::CursorShape hoverStyle(const QPoint &pos) override;


private:
    QString m_text;
    QFont m_font;
};

#endif // SHAPETEXTITEM_H
