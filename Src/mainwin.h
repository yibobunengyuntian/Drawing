#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include "ui_mainwin.h"

class MainWin : public QWidget, public Ui_MainWin
{
    Q_OBJECT

public:
    MainWin(QWidget *parent = nullptr);

protected:
    void initialize();

    template<typename ShapeItemType>
    void registerShapeItem(const QString &name = "");

private:
    QButtonGroup *m_pBtnDrawingGroup = nullptr;
    QString m_savePath;
};

#endif // MAINWIN_H
