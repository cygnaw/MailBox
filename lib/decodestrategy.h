#ifndef DECODESTRATEGY_H
#define DECODESTRATEGY_H

#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QTextCodec>

class DecodeStrategy
{
protected:
    QTextCodec* codec;
public:
    DecodeStrategy(): codec(QTextCodec::codecForName("utf-8")) {}
    virtual ~DecodeStrategy() {}
    virtual void setCodec(const QString &name) {
        QTextCodec *tmp = QTextCodec::codecForName(name.toUtf8());
        if (tmp)
            codec = tmp;
    }
    virtual void decode(const QString &src, QString &to) {
        QByteArray tmp;
        tmp.append(src);
        to = codec->toUnicode(tmp);
    }
    virtual void decode(const QStringList &src, QString &to) {
        QByteArray tmp;
        tmp.append(src.join("\n"));
        to = codec->toUnicode(tmp);
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
