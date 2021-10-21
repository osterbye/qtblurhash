#include "decoder.h"

Decoder::Decoder(QObject *parent) : QObject(parent) { }

QImage Decoder::imageFromBlurHash(QByteArray *blurhash, int width, int height, int punch)
{
    if (!blurhash || blurhash->length() < 6)
        return QImage();

    const int sizeFlag = decodeToInt(blurhash, 0, 1);
    if (sizeFlag < 1)
        return QImage();

    const int numY = (sizeFlag / 9) + 1;
    const int numX = (sizeFlag % 9) + 1;

    if (blurhash->length() != 4 + 2 * numX * numY)
        return QImage();

    const int quantizedMaxValue = decodeToInt(blurhash, 1, 2);
    if (quantizedMaxValue == -1)
        return QImage();

    qreal maxValue = (static_cast<qreal>(quantizedMaxValue + 1)) / 166;
    if (punch > 1)
        maxValue *= punch;

    QVector<factor> colors;
    colors.reserve(numX * numY);

    int value = decodeToInt(blurhash, 2, 6);
    if (value == -1)
        return QImage();

    factor dc;
    decodeDC(value, &dc);
    colors.append(dc);
    for (int i = 1; i < (numX * numY); ++i) {
        int index = 4 + (i * 2);
        value = decodeToInt(blurhash, index, index + 2);
        if (value == -1)
            return QImage();

        factor ac;
        decodeAC(value, maxValue, &ac);
        colors.append(ac);
    }

    QImage blurImage(width, height, QImage::Format_RGB32);
    if (blurImage.isNull())
        return blurImage;

    QRgb *pixel = reinterpret_cast<QRgb *>(blurImage.bits());
    for (int y = 0; y < height; ++y) {
        qreal yPi = M_PI * y;

        for (int x = 0; x < width; ++x) {
            qreal xPi = M_PI * x;
            factor pixelColor = { 0, 0, 0 };

            for (int j = 0; j < numY; ++j) {
                qreal jCos = qFastCos((yPi * j) / height);

                for (int i = 0; i < numX; ++i) {
                    qreal basics = qFastCos((xPi * i) / width) * jCos;
                    int idx = i + j * numX;
                    pixelColor.r += colors.at(idx).r * basics;
                    pixelColor.g += colors.at(idx).g * basics;
                    pixelColor.b += colors.at(idx).b * basics;
                }
            }

            int intR = linearTosRGB(pixelColor.r);
            intR = qBound(0, intR, 255);
            int intG = linearTosRGB(pixelColor.g);
            intG = qBound(0, intG, 255);
            int intB = linearTosRGB(pixelColor.b);
            intB = qBound(0, intB, 255);

            *pixel = qRgb(intR, intG, intB);
            ++pixel;
        }
    }
    return blurImage;
}

int Decoder::decodeToInt(QByteArray *blurhash, int start, int end)
{
    int value = 0;
    for (int i = start; i < end; i++) {
        int index = -1;
        for (int j = 0; j < 83; j++) {
            if (characters[j] == blurhash->at(i)) {
                index = j;
                break;
            }
        }
        if (index == -1)
            return -1;

        value = value * 83 + index;
    }
    return value;
}

void Decoder::decodeDC(int value, factor *rgb)
{
    rgb->r = sRGBToLinear(value >> 16);
    rgb->g = sRGBToLinear((value >> 8) & 255);
    rgb->b = sRGBToLinear(value & 255);
}

void Decoder::decodeAC(int value, qreal maximumValue, factor *rgb)
{
    int quantR = value / (19 * 19);
    int quantG = (value / 19) % 19;
    int quantB = value % 19;

    rgb->r = signPow((static_cast<qreal>(quantR) - 9) / 9, 2.0) * maximumValue;
    rgb->g = signPow((static_cast<qreal>(quantG) - 9) / 9, 2.0) * maximumValue;
    rgb->b = signPow((static_cast<qreal>(quantB) - 9) / 9, 2.0) * maximumValue;
}
