#ifndef DRAWINGWGT_H
#define DRAWINGWGT_H

#include <QWidget>
#include <QPainter>
#include <QGridLayout>
#include <QSpacerItem>

#include "canvas.h"

class OverlayWidget : public QWidget
{
public:
    OverlayWidget(QWidget* parent) : QWidget(parent) {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_NoSystemBackground);
        m_opPoints.fill(QPoint(), 3);
    }

    void setHiddenRect(const bool &is)
    {
        m_isShowRect = !is;
        update();
    }

    void setRect(const QRect &rect)
    {
        m_rect = rect;
        QPoint pos = m_rect.topLeft();
        int w = rect.width();
        int h = rect.height();
        m_opPoints[0] = pos + QPoint(w + 6, h + 6);
        m_opPoints[1] = pos + QPoint(w + 6, h / 2);
        m_opPoints[2] = pos + QPoint(w / 2, h + 6);
        update();
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        QPen pen(Qt::black);

        pen.setWidth(1);
        painter.setPen(pen);
        painter.setBrush(Qt::white);

        foreach (QPoint opPoint, m_opPoints) {
            painter.drawRect(opPoint.x() - 5, opPoint.y() - 5, 10, 10);
        }

        if(m_isShowRect)
        {
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(m_rect);
        }
    }

private:
    QRect m_rect;
    QList<QPoint> m_opPoints;
    bool m_isShowRect = false;
};

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
    void setCanvasBGColor(const QColor &color)
    {
        m_pCanvas->setCanvasBGColor(color);
    }
    void setCanvasBGPixmap(const QPixmap &pixmap)
    {
        m_pCanvas->setCanvasBGPixmap(pixmap);
    }
    void undo()
    {
        m_pCanvas->undo();
    }
    void redo()
    {
        m_pCanvas->redo();
    }
    QPixmap exportPixmap()
    {
        return m_pCanvas->exportPixmap();
    }
    QVariantMap save()
    {
        return m_pCanvas->save();
    }
    void load(const QVariantMap &data)
    {
        m_pCanvas->load(data);
    }

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Canvas *m_pCanvas = nullptr;
    OverlayWidget *m_pOverlay = nullptr;

    bool m_isPress = false;
    int m_opIndex = -1;
    QPoint m_pressPos;
    QPoint m_offsetPos;
    QPoint m_currPos;
    QVector<QPoint> m_opPoints;

    QList<QLine> m_scaleLineList;
    int m_w;
    int m_h;
};

#endif // DRAWINGWGT_H
