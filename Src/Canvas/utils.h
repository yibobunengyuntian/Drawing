#ifndef UTILS_H
#define UTILS_H

#include <QPointF>
#include <QLine>
#include <QPixmap>
#include <QPainter>
#include <QBuffer>
#include <QFrame>
#include <QVariant>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSettings>
#include <QApplication>
#include <QScreen>
#include <QDir>
#include <QFileInfo>

class Utils
{
public:
    Utils();

    // 计算一个点绕一点旋转后的位置
    static QPointF rotate(const QPointF &p, const float &angle, const QPointF &base);

    // 改变图片中不透明部分的颜色
    static QPixmap replaceOpaqueColor(const QPixmap& inputPixmap, const QColor& newColor);

    // 颜色相似度判断
    static bool isColorSimilar(QRgb color1, QRgb color2, int tolerance);

    // 图片转base64
    static QString imageToBase64(const QImage &image);

    // base64转图片
    static QImage base64ToImage(const QString &base64);

    //加载样式表
    static void loadStyle(const QString & styleFile);

    //读取json
    static QVariantList readJson(const QString &path);
    //写入json
    static void writeJson(const QVariantList &datas, const QString &path);

    //获取当前时间
    static QString currentDateTimeString(const QString format = "yyyy-MM-dd hh:mm:ss");

    //写入配置文件
    static void writeConfig(const QString &cfg, const QString &key, const QVariant &value);
    //读取配置文件
    static QVariant readConfig(const QString &cfg, const QString &key);

};

#endif // UTILS_H
