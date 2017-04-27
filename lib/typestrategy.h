#ifndef TYPESTRATEGY_H
#define TYPESTRATEGY_H

#include <QString>
#include "decodestrategy.h"


class TypeStrategy {
protected:
    DecodeStrategy* decoder;
public:
    virtual void handle(QStringList::iterator &it,
                        const QStringList::iterator &end,
                        QString &result) const = 0;
    virtual void setDecoder(DecodeStrategy* decoder) { this->decoder = decoder; }
    virtual void setBoundary(const QString &boundary) {}
};

class TypeText: public TypeStrategy {
public:
    virtual void handle(QStringList::iterator &it,
                        const QStringList::iterator &end,
                        QString &result) const;
};

class TypeTextPlain: public TypeText {
};

class TypeTextHtml: public TypeText {
};

class TypeMultipart: public TypeStrategy {
protected:
    QString boundary;
public:
    virtual void setBoundary(const QString &boundary) { this->boundary = boundary; }
};

class TypeMultipartAlternative: public TypeMultipart {
public:
    void handle(QStringList::iterator &it,
                const QStringList::iterator &end,
                QString &result) const;
};

class TypeMultipartMixed: public TypeMultipart {
public:
    void handle(QStringList::iterator &it,
                const QStringList::iterator &end,
                QString &result) const;
};

#endif // TYPESTRATEGY_H
