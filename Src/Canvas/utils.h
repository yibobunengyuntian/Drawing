#ifndef UTILS_H
#define UTILS_H

#include <QPointF>
#include <QLine>
#include <QPixmap>
#include <QPainter>
#include <QBuffer>
#include <QFrame>

class Utils
{
public:
    Utils();

    // 计算一个点绕一点旋转后的位置
    static QPointF rotate(const QPointF &p, const float &angle, const QPointF &base);

    // 改变图片中不透明部分的颜色
    static QPixmap replaceOpaqueColorWithPainter(const QPixmap& inputPixmap, const QColor& newColor);

    // 颜色相似度判断
    static bool isColorSimilar(QRgb color1, QRgb color2, int tolerance);

    // 图片转base64
    static QString imageToBase64(const QImage &image);

    // base64转图片
    static QImage base64ToImage(const QString &base64);


};

#endif // UTILS_H
