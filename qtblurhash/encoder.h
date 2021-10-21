#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>
#include "common.h"

class Encoder : public QObject
{
    Q_OBJECT
public:
    explicit Encoder(QObject *parent = nullptr);

    const QVector<char> blurHashFromImage(int xComponents, int yComponents, const QImage *image,
                                          int scaleWidth = 0);

private:
    inline factor multiplyBasisFunction(int xComponent, int yComponent, const QImage *image);
    int encodeDC(const factor *rgb);
    int encodeAC(const factor *rgb, qreal maximumValue);
    void encodeInt(int value, int length, QVector<char> *destination);
};

#endif // ENCODER_H
