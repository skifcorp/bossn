#ifndef TAGS_H
#define TAGS_H

#include <QMap>
#include <QString>
#include <QObject>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QVariant>

class Tag
{
public:
    typedef QSharedPointer<Tag> Pointer;    
    typedef QWeakPointer<Tag>   WeakPointer;

    Tag(const QString& n, QObject * o=nullptr, const QString& rm=QString(), const QString& wm=QString() ):tag_name(n), read_object(o), read_method(rm), write_method(wm)
    {

    }

    Tag(const Tag& ) = delete;

  /*  QVariant setValue (QGenericArgument val0 = QGenericArgument(),
                   QGenericArgument val1 = QGenericArgument(),
                   QGenericArgument val2 = QGenericArgument(),
                   QGenericArgument val3 = QGenericArgument(),
                   QGenericArgument val4 = QGenericArgument(),
                   QGenericArgument val5 = QGenericArgument(),
                   QGenericArgument val6 = QGenericArgument(),
                   QGenericArgument val7 = QGenericArgument() );*/

    QVariant value (QGenericArgument val0 = QGenericArgument(),
                   QGenericArgument val1 = QGenericArgument(),
                   QGenericArgument val2 = QGenericArgument(),
                   QGenericArgument val3 = QGenericArgument(),
                   QGenericArgument val4 = QGenericArgument(),
                   QGenericArgument val5 = QGenericArgument(),
                   QGenericArgument val6 = QGenericArgument(),
                   QGenericArgument val7 = QGenericArgument() );


    void setReadMethod(const QString& n) {read_method = n;}
    void setReadObject(QObject * o)      {read_object = o;}

/*    void setWriteMethod(const QString& n) {write_method = n;}
    void setWriteObject(QObject * o) {write_object = o;}*/

    void setName(const QString& n) {tag_name = n;}
    void addArgument(const QVariant& arg)
    {
        args.append(arg);
    }
    QVariant arg(int );

private:
    QString tag_name;
    QObject * read_object;
    QObject * write_object;
    QString read_method;
    QString write_method;
    QList<QVariant> args;
};

typedef QMap<QString, Tag::Pointer> Tags;

Q_DECLARE_METATYPE(Tag::WeakPointer)

#endif
