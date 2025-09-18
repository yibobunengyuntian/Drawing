#include "tooltextitem.h"


ToolTextItem::ToolTextItem(const QPoint &startPoint, const QPoint &endPoint)
    :RectOpItem(startPoint, endPoint)
{
    // 初始化字体
    m_font.setFamily("Arial");
    m_font.setPointSize(12);

    drawing(startPoint, endPoint);
}

void ToolTextItem::setText(const QString &text)
{
    m_text = text;
    updatePath();
}

QString ToolTextItem::text() const
{
    return m_text;
}

void ToolTextItem::setFont(const QFont &font)
{
    m_font = font;
    updatePath();
}

void ToolTextItem::setAlignment(const Qt::AlignmentFlag &alignment)
{
    m_alignment = alignment;
    updatePath();
}

QFont ToolTextItem::font() const
{
    return m_font;
}

void ToolTextItem::paint(QPainter *painter, bool isShowOP)
{
    painter->save();
    painter->translate(m_pos);

    float w = m_opPath.boundingRect().width();
    float h = m_opPath.boundingRect().height();
    QPointF center = m_opPath.boundingRect().center();
    QRectF rect = QRectF(center - QPointF(w / 2.0f - 3, h / 2.0f - 3),
                         QSizeF(w - 6, h - 6));

    painter->setFont(this->font());
    QTextOption option;
    option.setWrapMode(QTextOption::WrapAnywhere); // 设置文本换行模式
    option.setTextDirection(Qt::LeftToRight); // 设置文本方向
    option.setAlignment(m_alignment);
    painter->drawText(rect, this->text(), option);

    if(isShowOP)
    {
        // 设置虚线样式
        QPen pen(Qt::black);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawPath(m_opPath);
        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);
        foreach (QPoint opPoint, m_opPoints) {
            painter->drawRect(opPoint.x() - 5, opPoint.y() - 5, 10, 10);
        }
    }
    painter->restore();
}

void ToolTextItem::updatePath()
{
    m_path.clear();
    m_path.addRect(QRect(m_opPoints.first(), m_opPoints.last()));

    RectOpItem::updatePath();
}
