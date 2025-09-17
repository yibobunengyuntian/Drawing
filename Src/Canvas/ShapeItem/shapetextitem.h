#ifndef SHAPETEXTITEM_H
#define SHAPETEXTITEM_H

#include "shaperectopitem.h"
#include <QFont>
#include <QString>

class ShapeTextItem : public ShapeRectOpItem
{
public:
    ShapeTextItem(const QPoint &startPoint, const QPoint &endPoint);


    void setText(const QString &text);

    QString text() const;

    void setFont(const QFont &font);
    void setAlignment(const Qt::AlignmentFlag &alignment);

    QFont font() const;

    virtual void paint(QPainter *painter, bool isShowOP = false) override;


protected:
    virtual void updatePath() override;

private:
    QString m_text;
    QFont m_font;
    Qt::Alignment m_alignment = Qt::AlignCenter;
};

#endif // SHAPETEXTITEM_H
