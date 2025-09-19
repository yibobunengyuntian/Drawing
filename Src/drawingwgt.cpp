#include "drawingwgt.h"


DrawingWgt::DrawingWgt(QWidget *parent)
    : QWidget(parent)
{
    initialize();
}

void DrawingWgt::initialize()
{
    setMouseTracking(true);
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setObjectName("gridLayout");
    gridLayout->setContentsMargins(40, 40, 20, 20);
    m_pCanvas = new Canvas(this);

    gridLayout->addWidget(m_pCanvas, 0, 0, 1, 1);

    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);

    m_opPoints.fill(QPoint(),  3);

    connect(m_pCanvas, SIGNAL(mouseMove(QString)), this, SIGNAL(mouseMove(QString)));
    connect(m_pCanvas, SIGNAL(showSelectedRect(QString)), this, SIGNAL(showSelectedRect(QString)));
    connect(m_pCanvas, SIGNAL(canvasSizeChanged(QString)), this, SIGNAL(canvasSizeChanged(QString)));

    m_pOverlay = new OverlayWidget(this);
    m_w = m_pCanvas->width();
    m_h = m_pCanvas->height();
}

void DrawingWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    QPoint pos = m_pCanvas->pos();

    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(Qt::white);

    painter.drawLines(m_scaleLineList);

    if(m_currPos.x() >= pos.x() && m_currPos.y() >= pos.y())
    {
        painter.setPen(Qt::red);
        painter.drawLine(m_currPos.x(), 19, m_currPos.x(), 5);
        painter.drawLine(19, m_currPos.y(), 5, m_currPos.y());
    }
}

void DrawingWgt::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    QPoint pos = m_pCanvas->pos();
    m_scaleLineList.clear();
    m_scaleLineList << QLine(22, 20, this->width(), 20) << QLine(20, 22, 20, this->height());
    for(int i = pos.x(); i <= this->width(); i += 10)
    {
        int y = 15;
        if((i - pos.x()) % 100 == 0)
        {
            y = 5;
        }
        m_scaleLineList << QLine(i, 19, i, y);
    }
    for(int i = pos.y(); i <= this->height(); i += 10)
    {
        int x = 15;
        if((i - pos.y()) % 100 == 0)
        {
            x = 5;
        }
        m_scaleLineList << QLine(19, i, x, i);
    }
    m_pOverlay->setGeometry(rect());
    m_pOverlay->setRect(QRect(m_pCanvas->pos().x(), m_pCanvas->pos().y(), m_w, m_h));
    m_opPoints[0] = m_pCanvas->pos() + QPoint(m_w + 6, m_h + 6);
    m_opPoints[1] = m_pCanvas->pos() + QPoint(m_w + 6, m_h / 2);
    m_opPoints[2] = m_pCanvas->pos() + QPoint(m_w / 2, m_h + 6);
}

void DrawingWgt::mousePressEvent(QMouseEvent *event)
{
    m_isPress = true;
    m_pressPos = event->pos();

    for(int i = 0; i < m_opPoints.count(); ++i)
    {
        if(QLineF(m_opPoints[i], m_currPos).length() < 6)
        {
            m_opIndex = i;
            break;
        }
    }

    if(m_opIndex != -1)
    {
        m_offsetPos = m_pressPos - m_opPoints.at(m_opIndex) + QPoint(6, 6);
    }

    QWidget::mousePressEvent(event);
}

void DrawingWgt::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPress = false;
    if(m_opIndex != -1)
    {
        m_pCanvas->setCanvasSize(m_w, m_h);
        m_opIndex = -1;
    }
    m_pOverlay->setHiddenRect(true);
    QWidget::mouseReleaseEvent(event);
}

void DrawingWgt::mouseMoveEvent(QMouseEvent *event)
{
    m_currPos = event->pos();

    if(m_isPress && m_opIndex != -1)
    {
        QPoint wh = m_currPos - m_pCanvas->pos() - m_offsetPos;
        int w = qMax(wh.x(), 100);
        int h = qMax(wh.y(), 100);

        switch (m_opIndex) {
        case 0:
        {
            m_w = w;
            m_h = h;
        }
            break;
        case 1:
            m_w = w;
            break;
        case 2:
            m_h = h;
            break;
        default:
            break;
        }

        m_pOverlay->setRect(QRect(m_pCanvas->pos().x(), m_pCanvas->pos().y(), m_w, m_h));
        m_opPoints[0] = m_pCanvas->pos() + QPoint(m_w + 6, m_h + 6);
        m_opPoints[1] = m_pCanvas->pos() + QPoint(m_w + 6, m_h / 2);
        m_opPoints[2] = m_pCanvas->pos() + QPoint(m_w / 2, m_h + 6);
        m_pOverlay->setHiddenRect(false);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        m_opIndex = -1;
        QVector<Qt::CursorShape> cursors;
        cursors << Qt::SizeFDiagCursor << Qt::SizeHorCursor << Qt::SizeVerCursor;
        for(int i = 0; i < m_opPoints.count(); ++i)
        {
            if(QLineF(m_opPoints[i], m_currPos).length() < 6)
            {
                setCursor(cursors[i]);
                break;
            }
        }
    }
    update();
    QWidget::mouseMoveEvent(event);
}
