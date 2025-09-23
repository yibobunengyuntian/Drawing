#ifndef TEXTMENU_H
#define TEXTMENU_H

#include <QWidget>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QFontDatabase>
#include <QButtonGroup>
#include <QPushButton>
#include <QStyledItemDelegate>

#include "tooltextitem.h"

class TextMenu: public QWidget
{
    Q_OBJECT
public:
    TextMenu(QWidget *parent = nullptr);

    void bindTextItem(std::shared_ptr<ToolTextItem> item);

protected:
    void initialize();

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent *event) override;

protected slots:
    void updateItem();

private:
    bool m_dragging;
    QPointF m_dragPosition;

    std::shared_ptr<ToolTextItem> m_bindTextItem = nullptr;

    QLineEdit *m_pText = nullptr;
    QComboBox *m_pFontFamily = nullptr;
    QComboBox *m_pFontSize = nullptr;

    QPushButton *m_pBtnBold = nullptr;
    QPushButton *m_pBtnItalic = nullptr;
    QPushButton *m_pBtnUnderline = nullptr;
    QPushButton *m_pBtnOverline = nullptr;
    QPushButton *m_pBtnStrikeOut = nullptr;

    QButtonGroup *m_pButtonGroup = nullptr;
    QPushButton *m_pBtnAlignLeft = nullptr;
    QPushButton *m_pBtnAlignCenter = nullptr;
    QPushButton *m_pBtnAlignRight = nullptr;
};

// 自定义委托类，用于设置每个项的字体
class FontFamilyDelegate : public QStyledItemDelegate
{
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        // 获取当前项的字体名称
        QString fontFamily = index.data(Qt::DisplayRole).toString();
        QFont font;

        // 检查字体是否支持中文
        if (QFontDatabase::hasFamily(fontFamily) && isFontSupportChinese(fontFamily)) {
            font = QFont(fontFamily, 10);
        } else {
            font = QFont("微软雅黑", 10);
        }

        // 设置绘制字体
        painter->setFont(font);

        // 绘制项背景
        if (opt.state & QStyle::State_Selected) {
            painter->fillRect(opt.rect, opt.palette.highlight());
            painter->setPen(opt.palette.highlightedText().color());
        } else {
            painter->fillRect(opt.rect, opt.palette.base());
            painter->setPen(opt.palette.text().color());
        }

        // 绘制文本 - 添加标识不支持中文的字体
        QString displayText = fontFamily;
        // if (!isFontSupportChinese(fontFamily)) {
        //     displayText += " (不支持中文)";
        // }

        painter->drawText(opt.rect, Qt::AlignLeft | Qt::AlignVCenter, " " + displayText);
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        Q_UNUSED(option)
        Q_UNUSED(index)
        return QSize(250, 30);
    }

private:
    // 检查字体是否支持中文
    bool isFontSupportChinese(const QString &fontFamily) const
    {
        QFont font(fontFamily);
        QFontMetrics fm(font);

        // 检查一些常用中文字符的显示支持
        // 您可以根据需要添加更多测试字符
        static const QString testChineseChars = "中文测试字体";

        for (const QChar &c : testChineseChars) {
            if (!fm.inFont(c)) {
                return false;
            }
        }

        return true;
    }
};

#endif // TEXTMENU_H
