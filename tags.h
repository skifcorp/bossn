#ifndef TAGS_H
#define TAGS_H

#include <QMap>
#include <QString>
#include <QObject>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QVariant>
#include <QDebug>

#include "sharedfromthis.h"


class FuncContext;

class Tag
{
public:
    typedef QSharedPointer<Tag> Pointer;
    typedef QWeakPointer<Tag>   WeakPointer;

    Tag(const QString& n ):tag_name(n)
    {

    }

    Tag(const Tag& ) = delete;

    QVariant func (const QString& fn,
                   QGenericArgument val0 = QGenericArgument(),
                   QGenericArgument val1 = QGenericArgument(),
                   QGenericArgument val2 = QGenericArgument(),
                   QGenericArgument val3 = QGenericArgument(),
                   QGenericArgument val4 = QGenericArgument(),
                   QGenericArgument val5 = QGenericArgument(),
                   QGenericArgument val6 = QGenericArgument(),
                   QGenericArgument val7 = QGenericArgument() );



    void setTagName(const QString& n) {tag_name = n;}
    QString tagName() const {return tag_name;}

    void appendFunc(const QString& fn, QObject * obj, const QString& method)
    {
        if (funcs.contains(fn)) {
            qWarning() << "function "<<fn<<" already exists!!!!"; return;
        }

        funcs.insert(fn, FuncContext(obj, method));
    }


    void appendArgument (const QString& fn, const QVariant& arg);
private:    

    struct FuncContext {
        QObject        * object;
        QString         method;
        QVariantList    args;

        FuncContext (QObject * o , const QString& m ) :  object(o), method(m)
        {   }

        FuncContext () : object(nullptr)
        {   }
        //int placeholdersCount() const;
    };

    QVariant execObject(FuncContext & fn, const QList<QGenericArgument>& passed_args);

    QVariant func (const QString& fn, const QList<QGenericArgument>& );

    typedef QMap<QString, FuncContext> Funcs;
    Funcs funcs;

    QString tag_name;

};

typedef QMap<QString, Tag::Pointer> Tags;

struct TagBindable
{
    Tag::WeakPointer tag;
    QString method;

    TagBindable(Tag::WeakPointer t, const QString& m) : tag(t), method(m) {}
    TagBindable() {}
};

struct TagPlaceholder
{
    uint arg_num;
    //Tag::WeakPointer tag;
    //TagPlaceholder(uint n, Tag::WeakPointer t):arg_num(n), tag(t) {}
    TagPlaceholder(uint n):arg_num(n) {}
    TagPlaceholder():arg_num(0) {}
    //TagPlaceholder(Tag::WeakPointer t):tag(t){}
    //TagPlaceholder(){}
};


Q_DECLARE_METATYPE(TagBindable)
Q_DECLARE_METATYPE(TagPlaceholder)

#endif
