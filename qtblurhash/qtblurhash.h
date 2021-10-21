#ifndef QTBLURHASH_H
#define QTBLURHASH_H

#include "qtblurhash_global.h"
#include <QObject>
#include <QVector>

class Decoder;
class Encoder;
class QImage;

class QTBLURHASH_EXPORT QtBlurHash : public QObject
{
    Q_OBJECT
public:
    QtBlurHash(QObject *parent = nullptr);

    QImage imageFromBlurHash(QByteArray *blurhash, int width, int height, int punch = 1);
    const QVector<char> blurHashFromImage(int xComponents, int yComponents, const QImage *image,
                                          int scaleWidth = 0);

private:
    Decoder *m_decoder;
    Encoder *m_encoder;
};

#endif // QTBLURHASH_H
