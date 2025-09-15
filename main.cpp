#include "mainwin.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(QApplication::applicationDirPath() + "/Resources/app.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = QLatin1String(file.readAll());
        file.close();
        qApp->setStyleSheet(styleSheet);
        qApp->processEvents();
    }

    MainWin w;
    w.show();
    return a.exec();
}
