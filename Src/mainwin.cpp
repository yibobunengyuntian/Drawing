#include "mainwin.h"

#include "utils.h"
#include "flowlayout.h"
#include "shapelineitem.h"
#include "shaperectitem.h"
#include "shapeellipseitem.h"
#include "shapetriangleitem.h"
#include "shaperighttriangleitem.h"
#include "shaperhombusitem.h"
#include "shapepentagonitem.h"
#include "shapehexagonitem.h"
#include "shapearrowsitem.h"
#include "shapearrowsrightitem.h"
#include "shapearrowsleftitem.h"
#include "shapearrowsupitem.h"
#include "shapearrowsdownitem.h"
#include "shaperoundedrectlabelitem.h"

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
    registerShapeItem<ShapePentagonItem>("五边形");
    registerShapeItem<ShapeHexagonItem>("六边形");
    registerShapeItem<ShapeArrowsItem>("箭头");
    registerShapeItem<ShapeArrowsLeftItem>("向左箭头");
    registerShapeItem<ShapeArrowsRightItem>("向右箭头");
    registerShapeItem<ShapeArrowsUpItem>("向上箭头");
    registerShapeItem<ShapeArrowsDownItem>("向下箭头");
    registerShapeItem<ShapeRoundedRectLabelItem>("圆角矩形标注");

    m_pLabelPosIcon->setPixmap(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/mousePos.png"));
    m_pLabelSelectedRectIcon->setPixmap(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/selectedRect.png"));
    m_pLabelSizeIcon->setPixmap(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/size.png"));

    m_pBtnOpen->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/open.png"));
    m_pBtnSave->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/save.png"));
    m_pBtnExport->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/export.png"));
    m_pBtnUndo->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/undo.png"));
    m_pBtnRedo->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/redo.png"));
    m_pBtnOpen->setToolTip("打开图纸");
    m_pBtnSave->setToolTip("保存图纸");
    m_pBtnExport->setToolTip("导出图片");
    m_pBtnUndo->setToolTip("撤销");
    m_pBtnRedo->setToolTip("恢复");
    m_pBtnOpen->setShortcut(QKeySequence("Ctrl+O"));
    m_pBtnSave->setShortcut(QKeySequence("Ctrl+S"));
    m_pBtnExport->setShortcut(QKeySequence("Ctrl+E"));
    m_pBtnUndo->setShortcut(QKeySequence("Ctrl+Z"));
    m_pBtnRedo->setShortcut(QKeySequence("Ctrl+Y"));
    m_pBtnUndo->setEnabled(false);
    m_pBtnRedo->setEnabled(false);

    m_pBtnPencil->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/pencil.png"));
    m_pBtnEraser->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/eraser.png"));
    m_pBtnFill->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/fill.png"));
    m_pBtnText->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/text.png"));
    m_pBtnPicture->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/picture.png"));
    m_pBtnSelect->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/select.png"));
    m_pBtnPencil->setToolTip("铅笔");
    m_pBtnEraser->setToolTip("橡皮");
    m_pBtnFill->setToolTip("填充");
    m_pBtnText->setToolTip("文本");
    m_pBtnPicture->setToolTip("图片");
    m_pBtnSelect->setToolTip("选择");
    m_pBtnPencil->setChecked(true);

    m_pBgType->addItems(QStringList() << "颜色" << "图片");
    m_pBtnBgPicture->hide();
    m_pBtnBgColor->setColor(Qt::white);

    m_pBtnDrawingGroup->addButton(m_pBtnPencil);
    m_pBtnDrawingGroup->addButton(m_pBtnEraser);
    m_pBtnDrawingGroup->addButton(m_pBtnFill);
    m_pBtnDrawingGroup->addButton(m_pBtnText);
    m_pBtnDrawingGroup->addButton(m_pBtnPicture);
    m_pBtnDrawingGroup->addButton(m_pBtnSelect);

    m_pLabelLineSize->setText(QString::number(m_pSliderLineSize->value()));
    m_pLabelEraserSize->setText(QString::number(m_pSliderEraserSize->value()));
    m_pDrawingWet->setPenSize(m_pSliderLineSize->value());
    m_pDrawingWet->setEraserSize(m_pSliderEraserSize->value());

    foreach (auto btn, m_pMenuWgt->findChildren<QAbstractButton*>()) {
        connect(btn, &QAbstractButton::pressed, this, [=](){
            m_pDrawingWet->cancelSelected();
        });
    }
    foreach (auto btn, m_pOPWgt->findChildren<QAbstractButton*>()) {
        connect(btn, &QAbstractButton::pressed, this, [=](){
            m_pDrawingWet->cancelSelected();
        });
    }

    connect(m_pDrawingWet, SIGNAL(mouseMove(QString)), m_pLabelPos, SLOT(setText(QString)));
    connect(m_pDrawingWet, SIGNAL(showSelectedRect(QString)), m_pLabelSelectedRect, SLOT(setText(QString)));
    connect(m_pDrawingWet, SIGNAL(canvasSizeChanged(QString)), m_pLabelSize, SLOT(setText(QString)));
    connect(m_pBtnPenColor, SIGNAL(colorChanged(QColor)), m_pDrawingWet, SLOT(setPenColor(QColor)));
    connect(m_pBtnFillColor, SIGNAL(colorChanged(QColor)), m_pDrawingWet, SLOT(setFillColor(QColor)));
    connect(m_pBtnBgColor, SIGNAL(colorChanged(QColor)), m_pDrawingWet, SLOT(setCanvasBGColor(QColor)));
    connect(m_pBtnUndo, SIGNAL(clicked(bool)), m_pDrawingWet, SLOT(undo()));
    connect(m_pBtnRedo, SIGNAL(clicked(bool)), m_pDrawingWet, SLOT(redo()));
    connect(m_pDrawingWet, SIGNAL(canUndoChanged(bool)), m_pBtnUndo, SLOT(setEnabled(bool)));
    connect(m_pDrawingWet, SIGNAL(canRedoChanged(bool)), m_pBtnRedo, SLOT(setEnabled(bool)));
    connect(m_pBtnBgPicture, SIGNAL(clicked(bool)), this, SLOT(onSelectedBgPicture()));
    connect(m_pBtnExport, SIGNAL(clicked(bool)), this, SLOT(onExport()));
    connect(m_pBtnOpen, SIGNAL(clicked(bool)), this, SLOT(onOpen()));
    connect(m_pBtnSave, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(m_pBtnPencil, &QPushButton::clicked, this, [=](){
        m_pDrawingWet->setDrawingTool(Canvas::Pencil);
    });
    connect(m_pBtnEraser, &QPushButton::clicked, this, [=](){
        m_pDrawingWet->setDrawingTool(Canvas::Eraser);
    });
    connect(m_pBtnFill, &QPushButton::clicked, this, [=](){
        m_pDrawingWet->setDrawingTool(Canvas::Fill);
    });
    connect(m_pBtnText, &QPushButton::clicked, this, [=](){
        m_pDrawingWet->setDrawingTool(Canvas::Text);
    });
    connect(m_pBtnPicture, &QPushButton::clicked, this, [=](){
        m_pDrawingWet->setDrawingTool(Canvas::Picture);
    });
    connect(m_pBtnSelect, &QPushButton::clicked, this, [=](){
        m_pDrawingWet->setDrawingTool(Canvas::Select);
    });
    connect(m_pBgType, &QComboBox::currentIndexChanged, this, [=](int index){
        m_pBtnBgColor->setHidden(index != 0);
        m_pBtnBgPicture->setHidden(index != 1);
    });

    connect(m_pSliderLineSize, &QSlider::valueChanged, this, [=](int value){
        m_pLabelLineSize->setText(QString::number(value));
        m_pDrawingWet->setPenSize(value);
    });
    connect(m_pSliderEraserSize, &QSlider::valueChanged, this, [=](int value){
        m_pLabelEraserSize->setText(QString::number(value));
        m_pDrawingWet->setEraserSize(value);
    });

}

void MainWin::closeEvent(QCloseEvent *event)
{
    if(!m_pDrawingWet->isSaved())
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("未保存的更改");
        msgBox.setText("图纸已修改，是否保存？");
        // msgBox.setIcon(QMessageBox::Question);
        msgBox.setStyleSheet("QMessageBox { background-color: #f0f0f0; }");

        QPushButton *saveButton = msgBox.addButton("保存", QMessageBox::AcceptRole);
        QPushButton *discardButton = msgBox.addButton("不保存", QMessageBox::DestructiveRole);
        QPushButton *cancelButton = msgBox.addButton("取消", QMessageBox::RejectRole);

        msgBox.setDefaultButton(saveButton);
        msgBox.setEscapeButton(cancelButton);

        msgBox.exec();

        if (msgBox.clickedButton() == saveButton)
        {
            if(onSave())
            {
                event->accept();
            }
            else
            {
                event->ignore();
            }
        } else if (msgBox.clickedButton() == discardButton)
        {
            event->accept();
        } else
        {
            event->ignore();
        }
    }
}

bool MainWin::onSave()
{
    if(m_savePath.isEmpty())
    {
        m_savePath = QFileDialog::getSaveFileName(
            this,
            "保存图纸",
            Utils::readConfig(QApplication::applicationDirPath() + "/ini.cfg", "saveDir").toString(),
            "图纸文件 (*.drawing)"
            );
    }
    if(!m_savePath.isEmpty())
    {
        QVariantList vList;
        vList.append(m_pDrawingWet->save());
        Utils::writeJson(vList, m_savePath);
        Utils::writeConfig(QApplication::applicationDirPath() + "/ini.cfg", "saveDir", QFileInfo(m_savePath).dir().path());
        return true;
    }
    return false;
}

bool MainWin::onOpen()
{
    if(!m_pDrawingWet->isSaved())
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("未保存的更改");
        msgBox.setText("图纸已修改，是否保存？");
        // msgBox.setIcon(QMessageBox::Question);
        msgBox.setStyleSheet("QMessageBox { background-color: #f0f0f0; }");

        QPushButton *saveButton = msgBox.addButton("保存", QMessageBox::AcceptRole);
        QPushButton *discardButton = msgBox.addButton("不保存", QMessageBox::DestructiveRole);
        QPushButton *cancelButton = msgBox.addButton("取消", QMessageBox::RejectRole);

        msgBox.setDefaultButton(saveButton);
        msgBox.setEscapeButton(cancelButton);

        msgBox.exec();

        if (msgBox.clickedButton() == saveButton)
        {
            if(onSave())
            {

            }
            else
            {
                return false;
            }
        } else if (msgBox.clickedButton() == discardButton)
        {

        } else
        {
            return false;
        }
    }

    QString fileName = QFileDialog::getOpenFileName(
        this,
        "打开图纸",
        Utils::readConfig(QApplication::applicationDirPath() + "/ini.cfg", "saveDir").toString(),
        "图纸文件 (*.drawing)"
        );
    if(!fileName.isEmpty())
    {
        QVariantList vList = Utils::readJson(fileName);
        if(!vList.isEmpty())
        {
            m_pDrawingWet->load(vList.first().toMap());
        }
        m_savePath = fileName;
        return true;
    }
    return false;
}

bool MainWin::onExport()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "导出图片",
        Utils::readConfig(QApplication::applicationDirPath() + "/ini.cfg", "exportDir").toString(),
        "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)"
        );
    if(!fileName.isEmpty())
    {
        m_pDrawingWet->exportPixmap().save(fileName);
        Utils::writeConfig(QApplication::applicationDirPath() + "/ini.cfg", "exportDir", QFileInfo(fileName).dir().path());
        return true;
    }
    return false;
}

bool MainWin::onSelectedBgPicture()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择图片",
        Utils::readConfig(QApplication::applicationDirPath() + "/ini.cfg", "pictureDir").toString(),
        "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)"
        );
    if(!fileName.isEmpty())
    {
        m_pDrawingWet->setCanvasBGPixmap(QPixmap(fileName));
        Utils::writeConfig(QApplication::applicationDirPath() + "/ini.cfg", "pictureDir", QFileInfo(fileName).dir().path());
        return true;
    }
    return false;
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
