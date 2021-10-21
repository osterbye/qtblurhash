#include "qtblurhash.h"
#include "decoder.h"
#include "encoder.h"

QtBlurHash::QtBlurHash(QObject *parent) : QObject(parent)
{
    m_decoder = new Decoder(this);
    m_encoder = new Encoder(this);
}

QImage QtBlurHash::imageFromBlurHash(QByteArray *blurhash, int width, int height, int punch)
{
    return m_decoder->imageFromBlurHash(blurhash, width, height, punch);
}

const QVector<char> QtBlurHash::blurHashFromImage(int xComponents, int yComponents,
                                                  const QImage *image, int scaleWidth)
{
    return m_encoder->blurHashFromImage(xComponents, yComponents, image, scaleWidth);
}
