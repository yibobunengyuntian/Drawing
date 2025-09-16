#ifndef DRAWINGWGT_H
#define DRAWINGWGT_H

#include <QWidget>
#include <QPainter>
#include <QGridLayout>
#include <QSpacerItem>

#include "canvas.h"

class DrawingWgt : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingWgt(QWidget *parent = nullptr);

    template<typename DrawingItemType>
    void setDrawingShape()
    {
        m_pCanvas->setDrawingShape<DrawingItemType>();
    }

protected:
    void initialize();

signals:
    void mouseMove(const QString &str);
    void showSelectedRect(const QString &str);
    void canvasSizeChanged(const QString &str);

public slots:
    void setDrawingTool(const Canvas::Tool &tool)
    {
        m_pCanvas->setTool(tool);
    }
    void setPenSize(const int &size)
    {
        m_pCanvas->setPenSize(size);
    }
    void setEraserSize(const int &size)
    {
        m_pCanvas->setEraserSize(size);
    }
    void cancelSelected()
    {
        m_pCanvas->cancelSelected();
    }
    void setPenColor(const QColor &color)
    {
        m_pCanvas->setPenColor(color);
    }
    void setFillColor(const QColor &color)
    {
        m_pCanvas->setFillColor(color);
    }

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Canvas *m_pCanvas = nullptr;

    bool m_isPress = false;
    int m_opIndex = -1;
    QPoint m_pressPos;
    QPoint m_offsetPos;
    QPoint m_currPos;
    QVector<QPoint> m_opPoints;

    QList<QLine> m_scaleLineList;
    QPainterPath m_scaleText;
};

#endif // DRAWINGWGT_H
