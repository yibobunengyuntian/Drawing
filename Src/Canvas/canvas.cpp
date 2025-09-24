#include "canvas.h"
#include "utils.h"
#include "tooltextitem.h"
#include "toolpictureitem.h"
#include "toolcorveitem.h"
#include "canvascommand.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    initialize();
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

    update();
}

void Canvas::undoStackPush()
{
    if(m_lastCanvasPixmap.toImage() == m_canvasPixmap.toImage())
    {
        return;
    }
    if(m_tool == Eraser && m_dt == DT_Tool)
    {
        QRect rect = m_currLinePath.boundingRect().toRect();
        rect.setTopLeft(rect.topLeft() - QPoint(m_eraser.width() + 10, m_eraser.width() + 10));
        rect.setBottomRight(rect.bottomRight() + QPoint(m_eraser.width() + 10, m_eraser.width() + 10));
        rect = rect.intersected(m_canvasPixmap.rect());
        DrawingCommand* command = new DrawingCommand(this, m_lastCanvasPixmap.copy(rect), m_canvasPixmap.copy(rect), rect.topLeft());
        m_pUndoStack->push(command);
    }
    else if (m_tool == Fill && m_dt == DT_Tool) {
        QRect rect = m_fillRect.intersected(m_canvasPixmap.rect());
        DrawingCommand* command = new DrawingCommand(this, m_lastCanvasPixmap.copy(rect), m_canvasPixmap.copy(rect), rect.topLeft());
        m_pUndoStack->push(command);
    }
    else if (m_tool == Select && m_dt == DT_Tool) {
        QRect rect = m_currDrawingItem->boundingRect().united(m_selectRect);
        rect.setTopLeft(rect.topLeft() - QPoint(m_pen.width() + 10, m_pen.width() + 10));
        rect.setBottomRight(rect.bottomRight() + QPoint(m_pen.width() + 10, m_pen.width() + 10));
        rect = rect.intersected(m_canvasPixmap.rect());
         DrawingCommand* command = new DrawingCommand(this, m_lastCanvasPixmap.copy(rect), m_canvasPixmap.copy(rect), rect.topLeft());
        m_pUndoStack->push(command);
    }
    else if (m_currDrawingItem) {
        QRect rect = m_currDrawingItem->boundingRect();
        rect.setTopLeft(rect.topLeft() - QPoint(m_pen.width() + 10, m_pen.width() + 10));
        rect.setBottomRight(rect.bottomRight() + QPoint(m_pen.width() + 10, m_pen.width() + 10));
        rect = rect.intersected(m_canvasPixmap.rect());
        DrawingCommand* command = new DrawingCommand(this, m_lastCanvasPixmap.copy(rect), m_canvasPixmap.copy(rect), rect.topLeft());
        m_pUndoStack->push(command);
    }
    m_lastCanvasPixmap = QPixmap(); // 重置
    m_isSaved = false;
}

void Canvas::drawingTool(const QPoint &pos)
{
    switch (m_tool) {
    case Pencil:
    {
        // m_pPainter->drawLine(m_lastPos, pos);
        // if(m_currLinePath.isEmpty())
        // {
        //     m_currLinePath.moveTo(m_lastPos);
        // }
        // m_currLinePath.lineTo(pos);
        if(!m_currDrawingItem)
        {
            m_currDrawingItem = std::make_shared<ToolCorveItem>(m_pressPos, pos);
        }
        else {
            m_currDrawingItem->drawing(m_pressPos, pos);
        }
    }
        break;
    case Eraser:
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
    }
        break;
    case Text:
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
        break;
    case Picture:
    {
        if(!m_currDrawingItem)
        {
            m_currDrawingItem = std::make_shared<ToolPictureItem>(m_pressPos, pos, m_picture);
        }
        else {
            m_currDrawingItem->drawing(m_pressPos, pos);
        }
    }
        break;
    case Select:
    {
        QPoint point = QPoint(qMin(qMax(pos.x(), 0), this->width() - 1), qMin(qMax(pos.y(), 0), this->height() - 1));
        if(!m_currDrawingItem)
        {
            m_currDrawingItem = std::make_shared<ToolPictureItem>(m_pressPos, point);
        }
        else {
            m_currDrawingItem->drawing(m_pressPos, point);
        }
        QPixmap pic = m_canvasPixmap.copy(m_currDrawingItem->boundingRect());
        std::static_pointer_cast<ToolPictureItem>(m_currDrawingItem)->setPicture(pic);
    }
    break;
    default:
        break;
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

QRect Canvas::fill()
{
    QImage image = m_canvasPixmap.toImage();
    if (image.isNull()) return QRect();
    if (image.format() != QImage::Format_ARGB32) {
        image = image.convertToFormat(QImage::Format_ARGB32);
    }

    QRgb oldColor = image.pixel(m_pressPos);
    QRgb newColor = m_fillColor.rgba();

    if (oldColor == newColor) return QRect();

    const QPoint directions[] = {
        QPoint(0, -1), QPoint(1, 0), QPoint(0, 1), QPoint(-1, 0)
    };
    const int directionCount = 4;

    QQueue<QPoint> pixelsToCheck;
    pixelsToCheck.enqueue(m_pressPos);

    const int width = image.width();
    const int height = image.height();

    int minX = m_pressPos.x();
    int maxX = m_pressPos.x();
    int minY = m_pressPos.y();
    int maxY = m_pressPos.y();

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

        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;

        for (int i = 0; i < directionCount; ++i) {
            QPoint neighbor = current + directions[i];
            pixelsToCheck.enqueue(neighbor);
        }
    }

    QRect boundingBox(minX, minY, maxX - minX + 1, maxY - minY + 1);
    m_pPainter->save();
    m_pPainter->setCompositionMode(QPainter::CompositionMode_Source);
    m_pPainter->drawImage(0, 0, image);
    m_pPainter->restore();
    update();

    return boundingBox;
}

void Canvas::endDrawing()
{
    if(m_tool == Pencil && m_dt == DT_Tool) {
        cancelSelected();
    }
    if(m_tool == Select && m_dt == DT_Tool && m_currDrawingItem) {
        QPixmap pic(m_currDrawingItem->boundingRect().size());
        pic.fill(Qt::transparent);
        m_pPainter->save();
        m_pPainter->setCompositionMode(QPainter::CompositionMode_Source);
        m_pPainter->drawPixmap(m_currDrawingItem->boundingRect(), pic);
        m_pPainter->restore();
        m_selectRect = m_currDrawingItem->boundingRect();
    }
}

void Canvas::setCanvasBGPixmap(const QPixmap &pix)
{
    if(pix.isNull())
    {
        return;
    }
    m_bgType = Pixmap;
    m_bgPixmap = pix;
    m_isSaved = false;
    update();
}

void Canvas::setCanvasBGColor(const QColor &color)
{
    m_bgType = FillColor;
    if(m_bgColor == color)
    {
        return;
    }
    m_bgColor = color;
    m_bgPixmap.fill(m_bgColor);
    m_isSaved = false;
    update();
}

void Canvas::setCanvasSize(const int &w, const int &h)
{
    this->setFixedSize(w, h);

    QPixmap pix = m_canvasPixmap;
    m_canvasPixmap.scaled(w, h);
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
        cursor = QCursor(Utils::replaceOpaqueColor(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/pencil.png"), m_pen.color()), 5, 28);
        break;
    case Eraser:
    {
        cursor = QCursor(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/eraser.png"), 9, 32);
    }
        break;
    case Fill:
        cursor = QCursor(Utils::replaceOpaqueColor(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/fill.png"), m_fillColor), 26, 15);
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
            Utils::readConfig(QApplication::applicationDirPath() + "/ini.cfg", "pictureDir").toString(),
            "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)"
            );
        if(!fileName.isEmpty())
        {
            m_picture.load(fileName);
            Utils::writeConfig(QApplication::applicationDirPath() + "/ini.cfg", "pictureDir", QFileInfo(fileName).dir().path());
        }
    }
        break;
    case Select:
        cursor = Qt::CrossCursor;
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
    if(m_currDrawingItem) {
        m_pPainter->setPen(m_pen);
        m_currDrawingItem->paint(m_pPainter.get());

        undoStackPush();

        m_currDrawingItem.reset();
        m_currDrawingItem = nullptr;
        update();
        emit showSelectedRect("");
        m_pTextMenu->hide();
    }
}

void Canvas::undo()
{
    m_pUndoStack->undo();
    update();
}

void Canvas::redo()
{
    m_pUndoStack->redo();
    update();
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

QVariantMap Canvas::save()
{
    QVariantMap data;
    data.insert("canvasPixmap", Utils::imageToBase64(m_canvasPixmap.toImage()));
    data.insert("bgPixmap", Utils::imageToBase64(m_bgPixmap.toImage()));
    m_isSaved = true;
    return data;
}

void Canvas::load(const QVariantMap &data)
{
    QPixmap pix = QPixmap::fromImage(Utils::base64ToImage(data.value("canvasPixmap").toString()));

    if(pix.isNull())
    {
        return;
    }
    m_bgPixmap = QPixmap::fromImage(Utils::base64ToImage(data.value("bgPixmap").toString()));
    this->setFixedSize(pix.size());
    initCanvas();
    m_pPainter->drawPixmap(0, 0, pix);
    clearUndoStack();
    m_isSaved = true;
    update();
}

void Canvas::drawingPixmap(const QPixmap &pixmap, const QPoint &pos)
{
    // 保存原来的合成模式
    QPainter::CompositionMode oldMode = m_pPainter->compositionMode();

    // 设置源复制模式（直接替换，不混合）
    m_pPainter->setCompositionMode(QPainter::CompositionMode_Source);

    // 绘制pixmap
    m_pPainter->drawPixmap(pos, pixmap);

    // 恢复原来的合成模式
    m_pPainter->setCompositionMode(oldMode);
    update();
}

void Canvas::clearUndoStack()
{
    m_pUndoStack->clear();
    m_lastCanvasPixmap = QPixmap();
}

bool Canvas::isSaved()
{
    return m_isSaved;
}

void Canvas::initialize()
{
    setMouseTracking(true);

    this->setFixedSize(800, 600);
    m_pUndoStack = new QUndoStack(this);
    m_bgPixmap = QPixmap(this->size());
    m_bgPixmap.fill(m_bgColor);
    initCanvas();
    setTool(Pencil);

    m_pen = QPen(Qt::black);
    m_pen.setCapStyle(Qt::RoundCap);
    m_pen.setJoinStyle(Qt::MiterJoin);
    m_eraser =  QPen(Qt::transparent);
    m_eraser.setCapStyle(Qt::RoundCap);
    m_eraser.setJoinStyle(Qt::RoundJoin);

    if(this->parentWidget())
    {
        m_pTextMenu = new TextMenu(this->parentWidget());
    }
    else
    {
        m_pTextMenu = new TextMenu(this);
    }
    m_pTextMenu->hide();

    connect(m_pUndoStack, SIGNAL(canUndoChanged(bool)), this, SIGNAL(canUndoChanged(bool)));
    connect(m_pUndoStack, SIGNAL(canRedoChanged(bool)), this, SIGNAL(canRedoChanged(bool)));
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    int w = this->width();
    int h = this->height();

    painter.setClipRect(0, 0, w, h);

    painter.drawPixmap(0, 0, w, h, m_gridPixmap);

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
    m_gridPixmap = QPixmap(this->size());
    QPainter painter(&m_gridPixmap);;
    int gridSize = 20; // 每个格子的大小
    int w = this->width();
    int h = this->height();
    for (int y = 0; y < h; y += gridSize) {
        for (int x = 0; x < w; x += gridSize) {
            // 判断当前格子位置，决定颜色
            bool isLight = ((x / gridSize) % 2) ^ ((y / gridSize) % 2);
            QColor color = isLight ? QColor(240, 240, 240) : QColor(200, 200, 200);

            painter.fillRect(x, y, gridSize, gridSize, color);
        }
    }

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

    m_currLinePath.clear();

    if(m_currDrawingItem)
    {
        if(!m_currDrawingItem->selectedOpPoint(m_pressPos))
        {
            cancelSelected();
        } else {
            m_isSelectedOp = true;
        }
    }
    else
    {
        m_lastCanvasPixmap = m_canvasPixmap;
    }

    if(m_dt == DT_Tool && m_tool == Fill)
    {
        m_fillRect = fill();
        undoStackPush(); // 填充操作立即提交
    }

    QWidget::mousePressEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPress = false;
    m_isSelectedOp = false;


    if(m_isDrawing)
    {
        endDrawing();
        m_isDrawing = false;
    }

    if(m_isChanged) {
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

    m_isDrawing = false;
    if(m_isPress)
    {
        if(m_dt == DT_Tool && m_tool == Eraser)
        {
            m_isChanged = true;
        }

        if(m_isSelectedOp)
        {
            m_currDrawingItem->move(currPos - m_lastPos);
        }
        else
        {
            m_isDrawing = true;
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
        if(m_currDrawingItem && !(m_dt == DT_Tool && m_tool == Pencil))
        {
            setCursor(m_currDrawingItem->hoverStyle(currPos));
        }
    }
    QWidget::mouseMoveEvent(event);
}

