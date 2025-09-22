#include "mainwin.h"

#include <QApplication>

#include "utils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Utils::loadStyle(QApplication::applicationDirPath() + "/Resources/app.qss");

    MainWin w;
    w.show();
    return a.exec();
}
