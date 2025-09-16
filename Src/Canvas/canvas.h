#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainterPath>
#include <QPainter>
#include <QMouseEvent>
#include <QUndoCommand>
#include <QUndoStack>
#include <QApplication>
#include <QQueue>

#include "shapebaseitem.h"

class Canvas: public QWidget
{
    Q_OBJECT

public:
    // 背景类型
    enum BgType
    {
        FillColor = 0, // 纯色
        Pixmap         // 图片
    };

    // 绘制操作类型
    enum DrawingType
    {
        DT_Tool = 0, // 工具
        DT_Shape     // 形状
    };

    // 工具类型
    enum Tool
    {
        Pencil = 0, // 铅笔
        Eraser,     // 橡皮
        Fill        // 填充
    };

    Canvas(QWidget *parent = nullptr);
    ~Canvas();

    // 设置背景图片
    void setCanvasBGPixmap(const QPixmap &pix);

    // 设置背景颜色
    void setCanvasBGColor(const QColor &color);

    // 设置画布大小
    void setCanvasSize(const int &w, const int &h);

    // 选择工具
    void setTool(const Tool &tool);

    // 选择形状
    template<typename ShapeItemType>
    void setDrawingShape()
    {
        m_dt = DT_Shape;
        m_createShapeFunc = [](QPoint startPos, QPoint endPos) {
            return std::make_shared<ShapeItemType>(startPos, endPos);
        };
        this->setCursor(Qt::CrossCursor);
    }

    // 设置笔的线宽
    void setPenSize(const int &size);

    // 设置橡皮宽度
    void setEraserSize(const int &size);

    // 设置画笔颜色
    void setPenColor(const QColor &color);

    // 设置填充颜色
    void setFillColor(const QColor &color);

    // 取消选中
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
    void fill();

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

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
