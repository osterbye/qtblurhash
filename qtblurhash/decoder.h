#ifndef DECODER_H
#define DECODER_H

#include <QImage>
#include <QObject>
#include "common.h"

class Decoder : public QObject
{
    Q_OBJECT
public:
    explicit Decoder(QObject *parent = nullptr);

    QImage imageFromBlurHash(QByteArray *blurhash, int width, int height, int punch = 1);

private:
    int decodeToInt(QByteArray *blurhash, int start, int end);
    void decodeDC(int value, factor *rgb);
    void decodeAC(int value, qreal maximumValue, factor *rgb);
};

#endif // DECODER_H
