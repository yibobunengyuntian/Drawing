#include "utils.h"

Utils::Utils() {}

QPointF Utils::rotate(const QPointF &p, const float &angle, const QPointF &base)
{
    QLineF l{base,p};
    l.setAngle(l.angle() - angle);
    return l.p2();
}

QPixmap Utils::replaceOpaqueColorWithPainter(const QPixmap &inputPixmap, const QColor &newColor)
{
    if (inputPixmap.isNull()) {
        return QPixmap();
    }

    // 创建输出图像（与输入图像相同的格式）
    QPixmap outputPixmap(inputPixmap.size());
    outputPixmap.fill(Qt::transparent);

    QPainter painter(&outputPixmap);

    // 设置合成模式：只绘制不透明区域
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, inputPixmap);

    // 设置合成模式：用新颜色替换源像素
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(outputPixmap.rect(), newColor);

    painter.end();

    return outputPixmap;
}

bool Utils::isColorSimilar(QRgb color1, QRgb color2, int tolerance)
{
    if (tolerance == 0) return color1 == color2;

    int rDiff = qAbs(qRed(color1) - qRed(color2));
    int gDiff = qAbs(qGreen(color1) - qGreen(color2));
    int bDiff = qAbs(qBlue(color1) - qBlue(color2));
    int aDiff = qAbs(qAlpha(color1) - qAlpha(color2));

    return (rDiff <= tolerance && gDiff <= tolerance &&
            bDiff <= tolerance && aDiff <= tolerance);
}

QString Utils::imageToBase64(const QImage &image)
{
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QString base64Image = imageData.toBase64();
    return base64Image;
}

QImage Utils::base64ToImage(const QString &base64)
{
    return QImage::fromData(QByteArray::fromBase64(base64.toLatin1()));
}
