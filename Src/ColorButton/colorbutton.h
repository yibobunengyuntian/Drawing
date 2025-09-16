#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QToolButton>
#include <QColorDialog>
#include <QPainter>

class ColorButton : public QToolButton { // 或 QPushButton
    Q_OBJECT
public:
    explicit ColorButton(QWidget *parent);

    void setColor(const QColor &color);

    QColor color() const { return m_color; }

signals:
    void colorChanged(const QColor &color);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_color = Qt::black;
};

#endif // COLORBUTTON_H
