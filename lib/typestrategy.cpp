#include "typestrategy.h"
#include "QDebug"
#include <QRegularExpression>


void TypeText::handle(QStringList::iterator &it,
                      const QStringList::iterator &end,
                      QString &result) const {
    QStringList list;
    for (; it != end; ++it)
        list << *it;
    decoder->decode(list, result);
}


void TypeMultipartAlternative::handle(QStringList::iterator &it,
                                      const QStringList::iterator &end,
                                      QString &result) const {
    DecodeStrategy* nextDecode = nullptr;
    TypeStrategy* nextType = nullptr;
    bool isHtml = false;
    QStringList::iterator begin;
    for (; !it->startsWith("--"+boundary); ++it) continue;

    while (it != end) {
        if (it->startsWith("--"+boundary)) {
            QString charset;
            for (; !it->isEmpty(); ++it) {
                if (it->contains("content-type", Qt::CaseInsensitive)) {
                    if (it->contains("text/html", Qt::CaseInsensitive)) {
                        nextType = new TypeTextHtml;
                        isHtml = true;
                    } else if (it->contains("text/plain", Qt::CaseInsensitive))
                        nextType = new TypeTextPlain;
                }
                if (it->contains("content-transfer-encoding", Qt::CaseInsensitive)) {
                    if (it->contains("7bit", Qt::CaseInsensitive))
                        nextDecode = new Decode7Bit;
                    else if (it->contains("8bit", Qt::CaseInsensitive))
                        nextDecode = new Decode8Bit;
                    else if (it->contains("binary", Qt::CaseInsensitive))
                        nextDecode = new DecodeBinary;
                    else if (it->contains("quoted-printable", Qt::CaseInsensitive))
                        nextDecode = new DecodeQuotedPrintable;
                    else if (it->contains("base64", Qt::CaseInsensitive))
                        nextDecode = new DecodeBase64;
                }
                if (it->contains("charset=")) {
                    QRegularExpression re("charset=\"(\\w+)\"");
                    QRegularExpressionMatch match = re.match(*it);
                    charset = match.captured(1);
                }
            }
            if (nextType == nullptr)
                nextType = new TypeTextPlain;
            if (nextDecode == nullptr)
                nextDecode = new Decode8Bit;


            nextDecode->setCodec(charset);
            nextType->setDecoder(nextDecode);
            begin = ++it;
            for (; !it->startsWith("--"+boundary); ++it) continue;
            nextType->handle(begin, it, result);
            delete nextDecode;
            delete nextType;
            nextDecode = nullptr;
            nextType = nullptr;
            if (isHtml || it->startsWith("--"+boundary+"--")) break;
        }
    }
    for (; !it->startsWith("--"+boundary+"--"); ++it) continue;
}

void TypeMultipartMixed::handle(QStringList::iterator &it, const QStringList::iterator &end, QString &result) const {
    DecodeStrategy* nextDecode = nullptr;
    TypeStrategy* nextType = nullptr;

    QStringList::iterator begin;
    for (; !it->startsWith("--"+boundary); ++it) continue;


    for (; !it->isEmpty(); ++it) {
        if (it->contains("content-type", Qt::CaseInsensitive)) {
            if (it->contains("text/html", Qt::CaseInsensitive))
                nextType = new TypeTextHtml;
            else if (it->contains("text/plain", Qt::CaseInsensitive))
                nextType = new TypeTextPlain;
        }
        if (it->contains("content-transfer-encoding", Qt::CaseInsensitive)) {
            if (it->contains("7bit", Qt::CaseInsensitive))
                nextDecode = new Decode7Bit;
            else if (it->contains("8bit", Qt::CaseInsensitive))
                nextDecode = new Decode8Bit;
            else if (it->contains("binary", Qt::CaseInsensitive))
                nextDecode = new DecodeBinary;
            else if (it->contains("quoted-printable", Qt::CaseInsensitive))
                nextDecode = new DecodeQuotedPrintable;
            else if (it->contains("base64", Qt::CaseInsensitive))
                nextDecode = new DecodeBase64;
        }
    }
    if (nextType == nullptr)
        nextType = new TypeTextPlain;
    if (nextDecode == nullptr)
        nextDecode = new Decode8Bit;

    nextType->setDecoder(nextDecode);
    begin = ++it;
    for (; !it->startsWith("--"+boundary); ++it) continue;
    nextType->handle(begin, it, result);
    delete nextDecode;
    delete nextType;

    for (; !it->startsWith("--"+boundary+"--"); ++it) continue;
//    decoder->decode(result, result);
}
