#include "utils.h"

Utils::Utils() {}

QPointF Utils::rotate(const QPointF &p, const float &angle, const QPointF &base)
{
    QLineF l{base,p};
    l.setAngle(l.angle() - angle);
    return l.p2();
}

QPixmap Utils::replaceOpaqueColor(const QPixmap &inputPixmap, const QColor &newColor)
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
    int alphaDiff = qAbs(qAlpha(color1) - qAlpha(color2));
    int redDiff = qAbs(qRed(color1) - qRed(color2));
    int greenDiff = qAbs(qGreen(color1) - qGreen(color2));
    int blueDiff = qAbs(qBlue(color1) - qBlue(color2));

    return (alphaDiff + redDiff + greenDiff + blueDiff) <= tolerance;
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

void Utils::loadStyle(const QString &styleFile)
{
    QFile file(styleFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = QLatin1String(file.readAll());
        file.close();
        qApp->setStyleSheet(styleSheet);
        qApp->processEvents();
    }
}

QVariantList Utils::readJson(const QString &path)
{
    QVariantList datas;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        return datas;
    }
    QByteArray data = file.readAll();
    file.close();
    if(data.isEmpty()){
        return QVariantList();
    }

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &jsonError));
    if(jsonError.error != QJsonParseError::NoError){
        return datas;
    }

    QVariant var = jsonDoc.toVariant();
    switch (var.typeId()) {
    case QMetaType::QVariantMap:
    {
        QVariantMap data = var.toMap();
        datas.append(data);
    }
    break;
    case QMetaType::QVariantList:
    {
        datas = var.toList();
    }
    default:
        break;
    }

    return datas;
}

void Utils::writeJson(const QVariantList &datas, const QString &path)
{
    QJsonDocument jsonDoc = QJsonDocument::fromVariant((QVariant)datas);
    QByteArray byteData = jsonDoc.toJson();
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)){
        return;
    }
    file.write(byteData);
    file.close();
}

QString Utils::currentDateTimeString(const QString format)
{
    return QDateTime::currentDateTime().toString(format);
}

void Utils::writeConfig(const QString &cfg, const QString &key, const QVariant &value)
{
    QSettings set(cfg,QSettings::IniFormat);
    set.setValue(key,value);
}

QVariant Utils::readConfig(const QString &cfg, const QString &key)
{
    QSettings set(cfg,QSettings::IniFormat);
    return set.value(key);
}
