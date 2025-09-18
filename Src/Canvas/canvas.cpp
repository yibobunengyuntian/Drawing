#include "canvas.h"
#include "utils.h"
#include "tooltextitem.h"
#include "toolpictureitem.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);

    this->setFixedSize(800, 600);
    m_bgPixmap = QPixmap(this->size());
    setCanvasBGColor(Qt::white);
    initCanvas();
    setTool(Pencil);

    m_pen = QPen(Qt::black);
    m_pen.setCapStyle(Qt::RoundCap);
    m_pen.setJoinStyle(Qt::MiterJoin);
    m_eraser =  QPen(Qt::transparent);
    m_eraser.setCapStyle(Qt::RoundCap);
    m_eraser.setJoinStyle(Qt::RoundJoin);
}

Canvas::~Canvas()
{

}

void Canvas::initCanvas()
{
    // 创建一个新的透明画布
    m_pPainter.reset();
    m_canvasPixmap = QPixmap(this->size());
    m_canvasPixmap.fill(Qt::transparent);
    m_pPainter = std::make_shared<QPainter>(&m_canvasPixmap);
    // m_pPainter->setRenderHint(QPainter::Antialiasing, true);
    m_pPainter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    m_pPainter->setRenderHint(QPainter::TextAntialiasing, true);

    if(m_pTextMenu == nullptr)
    {
        if(this->parentWidget())
        {
        m_pTextMenu = new TextMenu(this->parentWidget());
        }
        else
        {
            m_pTextMenu = new TextMenu(this);
        }
        m_pTextMenu->hide();
    }

    update();
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
    if(m_tool == Text)
    {
        if(!m_currDrawingItem)
        {
            auto textItem = std::make_shared<ToolTextItem>(m_pressPos, pos);
            m_pTextMenu->show();
            m_pTextMenu->move(40, 40);
            m_pTextMenu->bindTextItem(textItem);
            m_currDrawingItem = textItem;
        }
        else {
            m_currDrawingItem->drawing(m_pressPos, pos);
        }
    }
    if(m_tool == Picture)
    {
        if(!m_currDrawingItem)
        {
            auto pictureItem = std::make_shared<ToolPictureItem>(m_pressPos, pos, m_picture);
            m_currDrawingItem = pictureItem;
        }
        else {
            m_currDrawingItem->drawing(m_pressPos, pos);
        }
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
    if(pix.isNull())
    {
        return;
    }
    m_bgType = Pixmap;
    m_bgPixmap = pix;
}

void Canvas::setCanvasBGColor(const QColor &color)
{
    m_bgType = FillColor;
    m_bgColor = color;
    m_bgPixmap.fill(color);
    update();
}

void Canvas::setCanvasSize(const int &w, const int &h)
{
    this->setFixedSize(w, h);
    // setCanvasBGColor(m_bgColor);

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
    case Text:
        cursor = Qt::CrossCursor;
        break;
    case Picture:
    {
        cursor = Qt::CrossCursor;

        QString fileName = QFileDialog::getOpenFileName(
            this,
            "选择图片",
            QDir::homePath(),
            "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)"
            );
        if(!fileName.isEmpty())
        {
            m_picture.load(fileName);
        }
    }
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
        m_pTextMenu->hide();
    }
}

QPixmap Canvas::exportPixmap()
{
    QPixmap ep(this->size());
    ep.fill(Qt::transparent);
    QPainter per(&ep);
    per.drawPixmap(this->rect(), m_bgPixmap);
    per.drawPixmap(this->rect(), m_canvasPixmap);

    return ep;
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
        // return;
    }

    int w = this->width();
    int h = this->height();

    painter.setClipRect(0, 0, w, h);

    // 居中绘制
    painter.drawPixmap(0, 0, w, h, m_bgPixmap);

    painter.drawPixmap(0, 0, w, h, m_canvasPixmap);

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

