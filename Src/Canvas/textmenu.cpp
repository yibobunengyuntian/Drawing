#include "textmenu.h"

TextMenu::TextMenu(QWidget *parent)
    :QWidget(parent)
{
    initialize();
}

void TextMenu::bindTextItem(std::shared_ptr<ToolTextItem> item)
{
    if(!item)
    {
        return;
    }
    m_bindTextItem = item;
    m_pText->setText(m_bindTextItem->text());
    updateItem();
}

void TextMenu::initialize()
{
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags()); // 隐藏默认边框
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->setCursor(Qt::ArrowCursor);
    QHBoxLayout *pLayout = new QHBoxLayout;
    m_pText = new QLineEdit;
    m_pText->setToolTip("文本");
    m_pText->setFixedSize(200, 30);
    pLayout->addWidget(m_pText);

    m_pFontFamily = new QComboBox;
    m_pFontFamily->setToolTip("字体");
    m_pFontFamily->setFixedHeight(30);

    // 设置自定义委托
    m_pFontFamily->setItemDelegate(new FontFamilyDelegate);

    QStringList fontFamilies = QFontDatabase::families();
    m_pFontFamily->addItems(fontFamilies);

    int index = m_pFontFamily->findText("微软雅黑");
    if (index != -1) {
        m_pFontFamily->setCurrentIndex(index);
    }
    pLayout->addWidget(m_pFontFamily);

    m_pFontSize = new QComboBox;
    m_pFontSize->setEditable(true);
    m_pFontSize->setMinimumWidth(50);
    m_pFontSize->setFixedHeight(30);
    m_pFontSize->setToolTip("字号");

    // 添加常用字号
    QList<int> standardSizes = {8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72};
    for (int size : standardSizes) {
        m_pFontSize->addItem(QString::number(size));
    }
    m_pFontSize->setCurrentText("12");
    pLayout->addWidget(m_pFontSize);

    pLayout->addSpacing(5);

    m_pBtnBold = new QPushButton;
    m_pBtnBold->setToolTip("加粗");
    m_pBtnBold->setCheckable(true);
    m_pBtnBold->setFixedSize(30, 30);
    m_pBtnBold->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/bold.png"));
    pLayout->addWidget(m_pBtnBold);

    m_pBtnItalic = new QPushButton;
    m_pBtnItalic->setToolTip("斜体");
    m_pBtnItalic->setCheckable(true);
    m_pBtnItalic->setFixedSize(30, 30);
    m_pBtnItalic->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/italic.png"));
    pLayout->addWidget(m_pBtnItalic);

    m_pBtnUnderline = new QPushButton;
    m_pBtnUnderline->setToolTip("下划线");
    m_pBtnUnderline->setCheckable(true);
    m_pBtnUnderline->setFixedSize(30, 30);
    m_pBtnUnderline->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/underline.png"));
    pLayout->addWidget(m_pBtnUnderline);

    m_pBtnOverline = new QPushButton;
    m_pBtnOverline->setToolTip("上划线");
    m_pBtnOverline->setCheckable(true);
    m_pBtnOverline->setFixedSize(30, 30);
    m_pBtnOverline->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/overline.png"));
    pLayout->addWidget(m_pBtnOverline);

    m_pBtnStrikeOut = new QPushButton;
    m_pBtnStrikeOut->setToolTip("删除线");
    m_pBtnStrikeOut->setCheckable(true);
    m_pBtnStrikeOut->setFixedSize(30, 30);
    m_pBtnStrikeOut->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/strikeOut.png"));
    pLayout->addWidget(m_pBtnStrikeOut);

    pLayout->addSpacing(5);

    m_pButtonGroup = new QButtonGroup(this);

    m_pBtnAlignLeft = new QPushButton;
    m_pBtnAlignLeft->setToolTip("左对齐");
    m_pBtnAlignLeft->setCheckable(true);
    m_pBtnAlignLeft->setFixedSize(30, 30);
    m_pBtnAlignLeft->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/alignLeft.png"));
    m_pButtonGroup->addButton(m_pBtnAlignLeft, Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->addWidget(m_pBtnAlignLeft);

    m_pBtnAlignCenter = new QPushButton;
    m_pBtnAlignCenter->setToolTip("居中对齐");
    m_pBtnAlignCenter->setCheckable(true);
    m_pBtnAlignCenter->setChecked(true);
    m_pBtnAlignCenter->setFixedSize(30, 30);
    m_pBtnAlignCenter->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/alignCenter.png"));
    m_pButtonGroup->addButton(m_pBtnAlignCenter, Qt::AlignCenter);
    pLayout->addWidget(m_pBtnAlignCenter);

    m_pBtnAlignRight = new QPushButton;
    m_pBtnAlignRight->setToolTip("右对齐");
    m_pBtnAlignRight->setCheckable(true);
    m_pBtnAlignRight->setFixedSize(30, 30);
    m_pBtnAlignRight->setIcon(QPixmap(QApplication::applicationDirPath() + "/Resources/icons/alignRight.png"));
    m_pButtonGroup->addButton(m_pBtnAlignRight, Qt::AlignRight | Qt::AlignVCenter);
    pLayout->addWidget(m_pBtnAlignRight);

    this->setLayout(pLayout);
    this->setFixedSize(800, 50);

    connect(m_pText, SIGNAL(textChanged(QString)), this, SLOT(updateItem()));

    foreach (auto btn, this->findChildren<QAbstractButton*>()) {
        connect(btn, &QAbstractButton::clicked, this, [=](){
            updateItem();
        });
    }

    foreach (auto cbox, this->findChildren<QComboBox*>()) {
        connect(cbox, &QComboBox::currentTextChanged, this, [=](){
            updateItem();
        });
    }
}

void TextMenu::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition() - frameGeometry().topLeft();
    }
    event->accept();
}

void TextMenu::mouseMoveEvent(QMouseEvent* event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move((event->globalPosition() - m_dragPosition).toPoint());
        event->accept();
    }
}

void TextMenu::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        if (this->pos().y() < 0)
        {
            this->move(this->pos().x(), 0);
        }
        event->accept();
    }
}

void TextMenu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(Qt::white);
    QRect rect = this->rect();
    rect.setTopLeft(rect.topLeft() + QPoint(2, 2));
    rect.setBottomRight(rect.bottomRight() - QPoint(2, 2));
    painter.drawRoundedRect(rect, 8, 8);
}

void TextMenu::updateItem()
{
    if(!m_bindTextItem)
    {
        return;
    }

    m_bindTextItem->setText(m_pText->text());
    QFont font = m_bindTextItem->font();
    font.setFamily(m_pFontFamily->currentText());
    font.setPointSize(m_pFontSize->currentText().toInt());
    font.setBold(m_pBtnBold->isChecked());
    font.setItalic(m_pBtnItalic->isChecked());
    font.setUnderline(m_pBtnUnderline->isChecked());
    font.setOverline(m_pBtnOverline->isChecked());
    font.setStrikeOut(m_pBtnStrikeOut->isChecked());
    m_bindTextItem->setFont(font);
    m_bindTextItem->setAlignment((Qt::AlignmentFlag)m_pButtonGroup->checkedId());

    if(this->parentWidget())
    {
        this->parentWidget()->update();
    }
}
