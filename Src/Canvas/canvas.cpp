#include "canvas.h"
#include "utils.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);

    this->setFixedSize(800, 600);
    setCanvasBGColor(Qt::white);
    initCanvas();
}

Canvas::~Canvas()
{

}

void Canvas::initCanvas()
{
    // 创建一个新的透明画布
    m_pPainter.reset();
    m_canvasPixmap = QPixmap(m_bgPixmap.size());
    m_canvasPixmap.fill(Qt::transparent);
    m_pPainter = std::make_shared<QPainter>(&m_canvasPixmap);
    // m_pPainter->setRenderHint(QPainter::Antialiasing, true);
    m_pPainter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    m_pPainter->setRenderHint(QPainter::TextAntialiasing, true);

    m_pPainter->translate(-m_offsetPos);

    m_pen = QPen(Qt::black);
    m_pen.setCapStyle(Qt::RoundCap);
    m_pen.setJoinStyle(Qt::RoundJoin);

    // 设置橡皮擦样式
    m_eraser =  QPen(Qt::transparent);
    m_eraser.setCapStyle(Qt::RoundCap);
    m_eraser.setJoinStyle(Qt::RoundJoin);

    setTool(Pencil);
    update();  // 触发界面重绘
}

void Canvas::undoStackPush()
{
    // if(m_lastCanvasPixmap.toImage() != m_canvasPixmap.toImage())
    //     m_undoStack.push(new DrawingCommand(this, m_lastCanvasPixmap, m_canvasPixmap));
}

void Canvas::drawingTool(const QPoint &pos)
{
    if(m_tool == Pencil)
    {
        m_pPainter->drawLine(m_lastPos, pos);
        if(m_currLinePath.isEmpty())
        {
            m_currLinePath.moveTo(m_lastPos);
        }
        m_currLinePath.lineTo(pos);
    }
    if(m_tool == Eraser)
    {
        m_pPainter->save();
        m_pPainter->setCompositionMode(QPainter::CompositionMode_Clear);
        m_pPainter->setPen(m_eraser);
        m_pPainter->drawLine(m_lastPos, pos);
        if(m_currLinePath.isEmpty())
        {
            m_currLinePath.moveTo(m_lastPos);
        }
        m_currLinePath.lineTo(pos);
        m_pPainter->restore();
        m_pPainter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    }
}

void Canvas::drawingShape(const QPoint &pos)
{
    if(m_createShapeFunc)
    {
        if(!m_currDrawingItem)
        {
            m_currDrawingItem = m_createShapeFunc(m_pressPos, pos);
        }
        else {
            m_currDrawingItem->drawing(m_pressPos, pos);
        }
    }
}

void Canvas::fill()
{
    QImage image = m_canvasPixmap.toImage();
    if (image.isNull()) return;

    QRgb oldColor = image.pixel(m_pressPos);
    QRgb newColor = qRgba(m_fillColor.red(), m_fillColor.green(), m_fillColor.blue(), 255);

    if (oldColor == newColor) return;

    const QPoint directions[] = {
        QPoint(0, -1), QPoint(1, 0), QPoint(0, 1), QPoint(-1, 0)
    };
    const int directionCount = 4;

    QQueue<QPoint> pixelsToCheck;
    pixelsToCheck.enqueue(m_pressPos);

    const int width = image.width();
    const int height = image.height();

    while (!pixelsToCheck.empty()) {
        QPoint current = pixelsToCheck.dequeue();

        int x = current.x();
        int y = current.y();

        if (x < 0 || x >= width || y < 0 || y >= height)
            continue;

        QRgb currentColor = image.pixel(x, y);

        if (!Utils::isColorSimilar(currentColor, oldColor, 70))
            continue;

        if (currentColor == newColor)
            continue;

        image.setPixel(x, y, newColor);

        for (int i = 0; i < directionCount; ++i) {
            QPoint neighbor = current + directions[i];
            pixelsToCheck.enqueue(neighbor);
        }
    }

    m_pPainter->drawImage(0, 0, image);
    update();
}

void Canvas::setCanvasBGPixmap(const QPixmap &pix)
{
    m_bgType = Pixmap;
    m_bgPixmap = pix;
    this->setFixedSize(m_bgPixmap.size());


    if(m_bgPixmap.width() * 1.0 / m_bgPixmap.height() > this->width() * 1.0 / this->height())
    {
        m_bgPixmap = m_bgPixmap.scaledToWidth(this->width(), Qt::SmoothTransformation);
    }
    else
    {
        m_bgPixmap = m_bgPixmap.scaledToHeight(this->height(), Qt::SmoothTransformation);
    }

    m_offsetPos = this->rect().center() - m_bgPixmap.rect().center();

    initCanvas();
}

void Canvas::setCanvasBGColor(const QColor &color)
{
    m_bgType = FillColor;
    m_bgColor = color;
    m_bgPixmap = QPixmap(size());
    m_bgPixmap.fill(color);
    update();
}

void Canvas::setCanvasSize(const int &w, const int &h)
{
    if(m_bgType == Pixmap)
    {
        return;
    }

    this->setFixedSize(w, h);
    setCanvasBGColor(m_bgColor);

    QPixmap pix = m_canvasPixmap;
    initCanvas();
    m_pPainter->drawPixmap(0, 0, pix);
}

void Canvas::setTool(const Tool &tool)
{
    m_dt = DT_Tool;
    m_tool = tool;

    QCursor cursor = Qt::ArrowCursor;
    switch (m_tool) {
    case Pencil:
        cursor = QCursor(Utils::replaceOpaqueColorWithPainter(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/pencil.png"), m_pen.color()), 5, 28);
        break;
    case Eraser:
    {
        cursor = QCursor(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/eraser.png"), 12, 29);
    }
        break;
    case Fill:
        cursor = QCursor(Utils::replaceOpaqueColorWithPainter(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/fill.png"), m_fillColor), 26, 15);
        break;
    default:
        break;
    }

    this->setCursor(cursor);
}

void Canvas::setPenSize(const int &size)
{
    m_pen.setWidth(size);
    update();
}

void Canvas::setEraserSize(const int &size)
{
    m_eraser.setWidth(size);
    update();
}

void Canvas::setPenColor(const QColor &color)
{
    m_pen.setColor(color);
    if(m_dt == DT_Tool)
    {
        setTool(m_tool);
    }
    update();
}

void Canvas::setFillColor(const QColor &color)
{
    m_fillColor = color;
    if(m_dt == DT_Tool)
    {
        setTool(m_tool);
    }
    update();
}

void Canvas::cancelSelected()
{
    if(m_currDrawingItem)
    {
        m_currDrawingItem->paint(m_pPainter.get());
        m_currDrawingItem.reset();
        update();
        undoStackPush();
        emit showSelectedRect("");
    }
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    if (m_bgPixmap.isNull())
    {
        return;
    }

    painter.setClipRect(QRect(m_offsetPos, QSize(m_bgPixmap.width(), m_bgPixmap.height())));

    // 居中绘制
    painter.drawPixmap((width() - m_bgPixmap.width()) / 2.0,(height() - m_bgPixmap.height()) / 2.0, m_bgPixmap);

    painter.drawPixmap((width() - m_canvasPixmap.width()) / 2.0,(height() - m_canvasPixmap.height()) / 2.0, m_canvasPixmap);

    if(m_currDrawingItem)
    {
        painter.setPen(m_pen);
        m_currDrawingItem->paint(&painter, true);
    }

    QWidget::paintEvent(event);
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    emit canvasSizeChanged(QString("%1 × %2 像素").arg(this->width()).arg(this->height()));
}

void Canvas::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    emit mouseMove("");
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    m_isPress = true;
    m_pressPos = event->pos();
    m_lastPos = event->pos();
    m_pPainter->setPen(m_pen);
    m_lastCanvasPixmap = m_canvasPixmap;
    m_currLinePath.clear();

    if(m_currDrawingItem)
    {
        if(!m_currDrawingItem->selectedOpPoint(m_pressPos))
        {
            cancelSelected();
        }
        else
        {
            m_isSelectedOp = true;
        }
    }
    if(m_dt == DT_Tool && m_tool == Fill)
    {
        fill();
    }

    QWidget::mousePressEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPress = false;
    m_isSelectedOp = false;
    if(m_isChanged)
    {
        undoStackPush();
        m_isChanged = false;
    }

    QWidget::mouseReleaseEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currPos = event->pos();
    emit mouseMove(QString("%1, %2 像素").arg(currPos.x()).arg(currPos.y()));
    if(m_currDrawingItem)
    {
        emit showSelectedRect(QString("%1 × %2 像素").arg(m_currDrawingItem->boundingRect().width()).arg(m_currDrawingItem->boundingRect().height()));
    }
    else
    {
        emit showSelectedRect("");
    }

    if(m_isPress)
    {
        m_isChanged = true;

        if(m_isSelectedOp)
        {
            m_currDrawingItem->move(currPos - m_lastPos);
        }
        else
        {
            if(m_dt == DT_Shape)
            {
                drawingShape(currPos);
            }
            if(m_dt == DT_Tool)
            {
                drawingTool(currPos);
            }
        }

        m_lastPos = currPos;
        update();
    }
    else
    {
        if(m_currDrawingItem)
        {
            setCursor(m_currDrawingItem->hoverStyle(currPos));
        }
    }
    QWidget::mouseMoveEvent(event);
}

