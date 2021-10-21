#include "encoder.h"
#include <QImage>

Encoder::Encoder(QObject *parent) : QObject(parent) { }

const QVector<char> Encoder::blurHashFromImage(int xComponents, int yComponents,
                                               const QImage *image, int scaleWidth)
{
    const QImage *imagePtr;
    /* Scaling large images down will improve generation time significantly and will create
     * blurhashes of pretty much the same quality. */
    QImage img;
    if (scaleWidth > 0 && scaleWidth < image->width()) {
        img = image->scaledToWidth(scaleWidth, Qt::FastTransformation);
        imagePtr = &img;
    } else {
        imagePtr = image;
    }

    QVector<char> buffer;
    buffer.reserve(2 + 4 + (xComponents * yComponents - 1) * 2 + 1);

    if (xComponents < 1 || xComponents > 9 || yComponents < 1 || yComponents > 9)
        return buffer;

    QVector<factor> ac;
    ac.reserve(xComponents * yComponents);

    for (int y = 0; y < yComponents; ++y) {
        for (int x = 0; x < xComponents; ++x) {
            ac.append(multiplyBasisFunction(x, y, imagePtr));
        }
    }
    factor dc = ac.takeFirst();

    int sizeFlag = (xComponents - 1) + (yComponents - 1) * 9;
    encodeInt(sizeFlag, 1, &buffer);

    qreal maximumValue;
    if (!ac.isEmpty()) {
        qreal actualMaximumValue = 0;
        for (int i = 0; i < ac.count(); ++i) {
            actualMaximumValue = qMax(qFabs(ac.at(i).r), actualMaximumValue);
            actualMaximumValue = qMax(qFabs(ac.at(i).g), actualMaximumValue);
            actualMaximumValue = qMax(qFabs(ac.at(i).b), actualMaximumValue);
        }
        int quantisedMaximumValue = qBound(0, qFloor(actualMaximumValue * 166 - 0.5), 82);
        maximumValue = (static_cast<qreal>(quantisedMaximumValue) + 1) / 166;
        encodeInt(quantisedMaximumValue, 1, &buffer);
    } else {
        maximumValue = 1;
        encodeInt(0, 1, &buffer);
    }

    encodeInt(encodeDC(&dc), 4, &buffer);

    for (int i = 0; i < ac.count(); ++i)
        encodeInt(encodeAC(&ac.at(i), maximumValue), 2, &buffer);

    buffer.append('\0');
    return buffer;
}

factor Encoder::multiplyBasisFunction(int xComponent, int yComponent, const QImage *image)
{
    factor result = { 0, 0, 0 };
    qreal normalisation = (xComponent == 0 && yComponent == 0) ? 1 : 2;
    qreal xPi = M_PI * xComponent;
    qreal yPi = M_PI * yComponent;
    int width = image->width();
    int height = image->height();

    const QRgb *pixel = reinterpret_cast<const QRgb *>(image->constBits());
    for (int y = 0; y < height; y++) {
        qreal yCos = qFastCos(yPi * y / height);
        for (int x = 0; x < width; x++) {
            qreal basis = qFastCos(xPi * x / width) * yCos;
            result.r += basis * sRGBToLinear(qRed(*pixel));
            result.g += basis * sRGBToLinear(qGreen(*pixel));
            result.b += basis * sRGBToLinear(qBlue(*pixel));
            ++pixel;
        }
    }

    qreal scale = normalisation / (width * height);
    result.r *= scale;
    result.g *= scale;
    result.b *= scale;

    return result;
}

int Encoder::encodeDC(const factor *rgb)
{
    int roundedR = linearTosRGB(rgb->r);
    int roundedG = linearTosRGB(rgb->g);
    int roundedB = linearTosRGB(rgb->b);
    return (roundedR << 16) + (roundedG << 8) + roundedB;
}

int Encoder::encodeAC(const factor *rgb, qreal maximumValue)
{
    int quantR = qBound(0, qFloor(signPow(rgb->r / maximumValue, 0.5) * 9 + 9.5), 18);
    int quantG = qBound(0, qFloor(signPow(rgb->g / maximumValue, 0.5) * 9 + 9.5), 18);
    int quantB = qBound(0, qFloor(signPow(rgb->b / maximumValue, 0.5) * 9 + 9.5), 18);
    return quantR * 19 * 19 + quantG * 19 + quantB;
}

void Encoder::encodeInt(int value, int length, QVector<char> *destination)
{
    int divisor = 1;
    for (int i = 0; i < length - 1; ++i)
        divisor *= 83;

    for (int i = 0; i < length; ++i) {
        int digit = (value / divisor) % 83;
        divisor /= 83;
        destination->append(characters[digit]);
    }
}
