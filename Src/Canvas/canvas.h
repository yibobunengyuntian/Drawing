#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainterPath>
#include <QPainter>
#include <QMouseEvent>
#include <QUndoCommand>
#include <QUndoStack>
#include <QApplication>

#include <queue>

#include "shapebaseitem.h"

class Canvas: public QWidget
{
    Q_OBJECT

public:
    enum BgType
    {
        FillColor = 0,
        Pixmap
    };
    enum Tool
    {
        Pencil = 0,
        Eraser,
        Fill
    };
    enum DrawingType
    {
        DT_Tool = 0,
        DT_Shape
    };

    Canvas(QWidget *parent = nullptr);
    ~Canvas();

    void setCanvasBGPixmap(const QPixmap &pix);
    void setCanvasBGColor(const QColor &color);

    void setCanvasSize(const int &w, const int &h);
    void setTool(const Tool &tool);

    template<typename ShapeItemType>
    void setDrawingShape()
    {
        m_dt = DT_Shape;
        m_createShapeFunc = [](QPoint startPos, QPoint endPos) {
            return std::make_shared<ShapeItemType>(startPos, endPos);
        };
        this->setCursor(Qt::CrossCursor);
    }

    void setPenSize(const int &size);
    void setEraserSize(const int &size);
    void cancelSelected();

signals:
    void mouseMove(const QString &str);
    void showSelectedRect(const QString &str);
    void canvasSizeChanged(const QString &str);

protected:
    void initCanvas();
    void undoStackPush();
    void drawingTool(const QPoint &pos);
    void drawingShape(const QPoint &pos);

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void fill();
    void floodFil(const QPoint& startPos, const QColor& fillColor, int tolerance = 0);

    // 颜色相似度判断函数
    bool isColorSimilar(QRgb color1, QRgb color2, int tolerance);

private:
    BgType m_bgType = FillColor;
    QColor m_bgColor = Qt::white;
    QPixmap m_bgPixmap;
    QPixmap m_canvasPixmap;
    std::shared_ptr<QPainter> m_pPainter = nullptr;
    QPen m_pen;
    QPen m_eraser;
    QColor m_fillColor;
    QPoint m_offsetPos;

    bool m_isPress = false;
    bool m_isChanged = false;
    QPoint m_pressPos;
    QPoint m_lastPos;
    QUndoStack m_undoStack;
    QPixmap m_lastCanvasPixmap;

    QPainterPath m_currLinePath;
    std::shared_ptr<ShapeBaseItem> m_currDrawingItem = nullptr;
    bool m_isSelectedOp = false;

    DrawingType m_dt = DT_Tool;
    Tool m_tool = Pencil;
    std::function<std::shared_ptr<ShapeBaseItem>(QPoint, QPoint)> m_createShapeFunc = nullptr;
};

class DrawingCommand : public QUndoCommand
{
public:
    explicit DrawingCommand()
    {

    }
    ~DrawingCommand()
    {

    }
    void redo() override
    {

    }
    void undo() override
    {

    }

private:


};

#endif // CANVAS_H
