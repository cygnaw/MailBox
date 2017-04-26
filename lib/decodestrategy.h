#ifndef DECODESTRATEGY_H
#define DECODESTRATEGY_H

#include <QByteArray>
#include <QString>
#include <QStringList>

class DecodeStrategy
{
public:
    virtual void decode(const QString &src, QString &to) {
        to = src;
    }
    virtual void decode(const QStringList &src, QString &to) {
        to = src.join("\n");
    }
};

class Decode7Bit: public DecodeStrategy {
};

class Decode8Bit: public DecodeStrategy {
};

class DecodeBinary: public DecodeStrategy {
};

class DecodeQuotedPrintable: public DecodeStrategy {
public:
    void decode(const QString &src, QString &to);
    void decode(const QStringList &src, QString &to);
};

class DecodeBase64: public DecodeStrategy {
public:
    void decode(const QString &src, QString &to);
    void decode(const QStringList &src, QString &to);
};
#endif // DECODESTRATEGY_H
