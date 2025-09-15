#include "mainwin.h"

#include "flowlayout.h"
#include "shapelineitem.h"
#include "shaperectitem.h"
#include "shapeellipseitem.h"
#include "shapetriangleitem.h"
#include "shaperighttriangleitem.h"
#include "shaperhombusitem.h"

MainWin::MainWin(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    initialize();
}

void MainWin::initialize()
{
    this->setWindowTitle("Drawing");
    FlowLayout *pLayout = new FlowLayout(0, 5, 5);
    m_pShapeWgt->setLayout(pLayout);
    m_pBtnDrawingGroup = new QButtonGroup(this);

    registerShapeItem<ShapeLineItem>("直线");
    registerShapeItem<ShapeRectItem>("矩形");
    registerShapeItem<ShapeEllipseItem>("椭圆");
    registerShapeItem<ShapeTriangleItem>("三角形");
    registerShapeItem<ShapeRightTriangleItem>("直角三角形");
    registerShapeItem<ShapeRhombusItem>("菱形");

    m_pLabelPosIcon->setPixmap(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/mousePos.png"));
    m_pLabelSelectedRectIcon->setPixmap(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/selectedRect.png"));
    m_pLabelSizeIcon->setPixmap(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/size.png"));

    m_pBtnUndo->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/undo.png"));
    m_pBtnRedo->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/redo.png"));
    m_pBtnUndo->setToolTip("撤销");
    m_pBtnRedo->setToolTip("恢复");

    m_pBtnPencil->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/pencil.png"));
    m_pBtnEraser->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/eraser.png"));
    m_pBtnFill->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/fill.png"));
    m_pBtnPencil->setToolTip("铅笔");
    m_pBtnEraser->setToolTip("橡皮");
    m_pBtnFill->setToolTip("填充");
    m_pBtnPencil->setChecked(true);

    m_pBtnDrawingGroup->addButton(m_pBtnPencil);
    m_pBtnDrawingGroup->addButton(m_pBtnEraser);
    m_pBtnDrawingGroup->addButton(m_pBtnFill);

    connect(m_pDrawingWet, SIGNAL(mouseMove(QString)), m_pLabelPos, SLOT(setText(QString)));
    connect(m_pDrawingWet, SIGNAL(showSelectedRect(QString)), m_pLabelSelectedRect, SLOT(setText(QString)));
    connect(m_pDrawingWet, SIGNAL(canvasSizeChanged(QString)), m_pLabelSize, SLOT(setText(QString)));
    connect(m_pBtnPencil, &QPushButton::clicked, this, [=](){
        m_pDrawingWet->setDrawingTool(Canvas::Pencil);
    });
    connect(m_pBtnEraser, &QPushButton::clicked, this, [=](){
        m_pDrawingWet->setDrawingTool(Canvas::Eraser);
    });
    connect(m_pBtnFill, &QPushButton::clicked, this, [=](){
        m_pDrawingWet->setDrawingTool(Canvas::Fill);
    });

    m_pLabelLineSize->setText(QString::number(m_pSliderLineSize->value()));
    m_pLabelEraserSize->setText(QString::number(m_pSliderEraserSize->value()));
    m_pDrawingWet->setPenSize(m_pSliderLineSize->value());
    m_pDrawingWet->setEraserSize(m_pSliderEraserSize->value());
    connect(m_pSliderLineSize, &QSlider::valueChanged, this, [=](int value){
        m_pLabelLineSize->setText(QString::number(value));
        m_pDrawingWet->setPenSize(value);
    });
    connect(m_pSliderEraserSize, &QSlider::valueChanged, this, [=](int value){
        m_pLabelEraserSize->setText(QString::number(value));
        m_pDrawingWet->setEraserSize(value);
    });

    foreach (auto btn, this->findChildren<QAbstractButton*>()) {
        connect(btn, &QAbstractButton::pressed, this, [=](){
            m_pDrawingWet->cancelSelected();
        });
    }
}

template<typename ShapeItemType>
void MainWin::registerShapeItem(const QString &name)
{
    QPixmap pixmap = QPixmap(30, 30);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    QPen pen(Qt::black);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(2);
    painter.setPen(pen);
    ShapeItemType item(QPoint(5, 5), QPoint(25, 25));
    item.paint(&painter);

    QPushButton *btn = new QPushButton(m_pShapeWgt);
    btn->setCheckable(true);
    btn->setFixedSize(30, 30);
    btn->setIcon(pixmap);

    if(!name.isEmpty())
    {
        btn->setToolTip(name);
    }

    m_pShapeWgt->layout()->addWidget(btn);
    m_pBtnDrawingGroup->addButton(btn);

    connect(btn, &QPushButton::clicked, this, [this](bool is){
        if(is)
        {
            m_pDrawingWet->setDrawingShape<ShapeItemType>();
        }
    });
}
