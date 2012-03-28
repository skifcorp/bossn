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

    Tag(const QString& n, QObject * o=nullptr, const QString& rm=QString(), const QString& wm=QString() ):tag_name(n), object(o), read_method(rm), write_method(wm)
    {

    }

    //Tag():object(nullptr){}

    Tag(const Tag& ) = delete;

    QVariant value (QGenericArgument val0 = QGenericArgument(),
                   QGenericArgument val1 = QGenericArgument(),
                   QGenericArgument val2 = QGenericArgument(),
                   QGenericArgument val3 = QGenericArgument(),
                   QGenericArgument val4 = QGenericArgument(),
                   QGenericArgument val5 = QGenericArgument(),
                   QGenericArgument val6 = QGenericArgument(),
                   QGenericArgument val7 = QGenericArgument() );


    void setReadMethod(const QString& n) {read_method = n;}
    void setReadObject(QObject * o) {object = o;}
    void setName(const QString& n) {tag_name = n;}
    void addArgument(const QVariant& arg)
    {
        args.append(arg);
    }

private:

    QString tag_name;
    QObject * object;
    QString read_method;
    QString write_method;
    QList<QVariant> args;
};

typedef QMap<QString, Tag::Pointer> Tags;

#endif // TAGS_H
