#ifndef COMMON_H
#define COMMON_H

#include <QtMath>

typedef struct {
    qreal r;
    qreal g;
    qreal b;
} factor;

const char characters[84] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz#$%*+,-.:;=?@[]^_{|}~";

inline int linearTosRGB(qreal value)
{
    qreal v = qBound(0.0, value, 1.0);
    if (v <= 0.0031308) {
        v = v * 12.92 * 255 + 0.5;
    } else {
        v = (1.055 * qPow(v, 1 / 2.4) - 0.055) * 255 + 0.5;
    }
    return v;
}

inline qreal sRGBToLinear(int value)
{
    qreal v = static_cast<qreal>(value) / 255;
    if (v <= 0.04045) {
        v = v / 12.92;
    } else {
        v = qPow((v + 0.055) / 1.055, 2.4);
    }
    return v;
}

inline qreal signPow(qreal value, qreal exp)
{
    return copysign(qPow(qFabs(value), exp), value);
}

#endif // COMMON_H
