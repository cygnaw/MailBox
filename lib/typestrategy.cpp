#include "typestrategy.h"
#include "QDebug"


void TypeText::handle(QStringList::iterator &it,
                      const QStringList::iterator &end,
                      QString &result) const {
    QStringList list;
    for (; it != end && it+1 != end; ++it)
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
            for (; !it->isEmpty(); ++it) {
                if (it->contains("content-type", Qt::CaseInsensitive)) {
                    if (it->contains("text/html", Qt::CaseInsensitive)) {
                        nextType = new TypeTextHtml;
                        isHtml = true;
                    } else if (it->contains("text/plain", Qt::CaseInsensitive))
                        nextType = new TypeTextPlain;
                    else
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
                    else
                        nextDecode = new Decode8Bit;
                }
            }
            nextType->setDecoder(nextDecode);
            begin = ++it;
            for (; !it->startsWith("--"+boundary); ++it) continue;
            nextType->handle(begin, it, result);
            delete nextDecode;
            delete nextType;
            if (isHtml || it->startsWith("--"+boundary+"--")) break;
        }
    }

}
