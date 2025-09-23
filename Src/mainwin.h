#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QMessageBox>
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

    virtual void closeEvent(QCloseEvent *event) override;

protected slots:
    bool onSave();
    bool onOpen();
    bool onExport();
    bool onSelectedBgPicture();

private:
    QButtonGroup *m_pBtnDrawingGroup = nullptr;
    QString m_savePath;
};

#endif // MAINWIN_H
