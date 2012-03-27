#ifndef TAGS_H
#define TAGS_H

#include <QMap>
#include <QString>
#include <QObject>
#include <QSharedPointer>
#include <QVariant>

class Tag
{
public:
    typedef QSharedPointer<Tag> Pointer;

    Tag(QObject * o, const QString& rm, const QString& wm ):object(o), read_method(rm), write_method(wm)
    {

    }

    Tag():object(nullptr){}

    Tag(const Tag& ) = delete;

    QVariant value (QGenericArgument val0 = QGenericArgument(),
                   QGenericArgument val1 = QGenericArgument(),
                   QGenericArgument val2 = QGenericArgument(),
                   QGenericArgument val3 = QGenericArgument(),
                   QGenericArgument val4 = QGenericArgument(),
                   QGenericArgument val5 = QGenericArgument(),
                   QGenericArgument val6 = QGenericArgument(),
                   QGenericArgument val7 = QGenericArgument(),
                   QGenericArgument val8 = QGenericArgument(),
                   QGenericArgument val9 = QGenericArgument() )
    {
        QVariant ret;

        QMetaObject::invokeMethod( object, read_method.toAscii().data(), val0, val1, val2, val3, val4, val5, val6, val7, val8, val9 );

        return ret;
    }
/*
    template <class... Args>
    QVariant write (Args... args)
    {
        QVariant ret;

        invoke(write_method.toAscii().data(), Q_RETURN_ARG(QVariant, ret), Q_ARG(decltype(args), args)... );

        return ret;
    } */

    /*template <class ... Ars>
    QVariant value(Args ... args)
    {
        QVariant v;

        return v;
    }*/
    void setReadMethod(const QString& n) {read_method = n;}
    void setReadObject(QObject * o) {object = o;}
private:

    QString tag_name;
    QObject * object;
    QString read_method;
    QString write_method;
};

typedef QMap<QString, Tag::Pointer> Tags;

#endif // TAGS_H
