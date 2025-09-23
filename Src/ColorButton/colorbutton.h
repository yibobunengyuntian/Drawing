#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QToolButton>
#include <QColorDialog>
#include <QPainter>

class ColorButton : public QToolButton { // 或 QPushButton
    Q_OBJECT
public:
    explicit ColorButton(QWidget *parent = nullptr);

    void setColor(const QColor &color);

    QColor color() const { return m_color; }

signals:
    void colorChanged(const QColor &color);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
private:
    QColor m_color = Qt::black;
    QPixmap m_gridPixmap;
};

#endif // COLORBUTTON_H
