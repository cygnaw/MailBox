#include "decodestrategy.h"

void DecodeQuotedPrintable::decode(const QString &src, QString &to) {
    //                    0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?  @  A   B   C   D   E   F
    const int hexVal[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};
    QByteArray tmp;
    QByteArray decode_string;
    tmp.append(src);

    for (int i = 0; i < src.length(); ++i)
    {
        if (tmp.at(i) == '=')
            decode_string.append((hexVal[tmp.at(++i) - '0'] << 4) + hexVal[tmp.at(++i) - '0']);
        else
            decode_string.append(tmp.at(i));
    }

    to = codec->toUnicode(decode_string);
}

void DecodeQuotedPrintable::decode(const QStringList &src, QString &to) {
    QString tmp;
    foreach (QString s, src) {
        if (s.endsWith('='))
            s.chop(1);
        tmp += s;
    }
    decode(tmp, to);
}

void DecodeBase64::decode(const QString &src, QString &to) {
    QByteArray tmp;
    tmp.append(src);
    tmp = QByteArray::fromBase64(tmp);
    to = codec->toUnicode(tmp);
}

void DecodeBase64::decode(const QStringList &src, QString &to) {
    QByteArray tmp;
    tmp.append(src.join(""));
    tmp = QByteArray::fromBase64(tmp);
    to = codec->toUnicode(tmp);
}

