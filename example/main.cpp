#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QString>
#include "qtblurhash.h"

void usage(char *name)
{
    qDebug() << "Usage:\n\t" << name
             << " x_components y_components in_file out_file\n\n\tx_components and y_components "
                "has a max. value of 9\n\twhen creating a hash.\n\nExamples:\n\t"
             << name << " 9 6 mypic.jpg myhash.txt\n\t" << name
             << " 640 480 myhash.txt myblur.png\n";
    exit(1);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QtBlurHash qtBlurHash;

    if (5 != argc)
        usage(argv[0]);

    bool ok = true;
    int width = QString(argv[1]).toUInt(&ok);
    if (!ok)
        usage(argv[0]);

    int height = QString(argv[2]).toUInt(&ok);
    if (!ok)
        usage(argv[0]);

    QString inFileName(argv[3]);
    QImage img;
    if (inFileName.contains(".jpg", Qt::CaseInsensitive)
        || inFileName.contains(".png", Qt::CaseInsensitive)) {
        ok = img.load(inFileName);
        if (!ok) {
            qDebug() << "Loading image file failed";
            usage(argv[0]);
        }
        QVector<char> blurhash = qtBlurHash.blurHashFromImage(width, height, &img, 0);
        if (blurhash.isEmpty()) {
            qDebug() << "Creating blurhash failed";
            usage(argv[0]);
        }
        QFile outFile(argv[4]);
        if (outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            outFile.write(blurhash.constData());
            outFile.close();
        }
        qDebug() << blurhash.constData();
    } else {
        qDebug("decode\n");
        QFile inFile(inFileName);
        if (inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray blurhash = inFile.readAll().trimmed();
            inFile.close();
            img = qtBlurHash.imageFromBlurHash(&blurhash, width, height);
            if (img.isNull()) {
                qDebug() << "Decoding blurhash failed";
                usage(argv[0]);
            } else {
                img.save(argv[4]);
            }
        } else {
            qDebug() << "Loading blurhash file failed";
            usage(argv[0]);
        }
    }

    exit(0);
    return a.exec();
}
