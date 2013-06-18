#ifndef TAGMETHOD_H
#define TAGMETHOD_H


#include <QString>
#include <QVariant>
#include <QDebug>

enum class TagMethodType{
    Simple, CompareValue, Property
};


template <TagMethodType Typ>
struct TagMethod;



template <>
struct TagMethod<TagMethodType::Simple>
{
    QString tag_name;
    QString method_name;
    TagMethod(const QString& tn, const QString & mn):tag_name(tn), method_name(mn){}
    TagMethod(const QString &tn) : tag_name(tn) {}
    TagMethod(){}

    TagMethod( const TagMethod& ) = default;

    TagMethod( TagMethod&& other ) : tag_name( std::move( other.tag_name ) ), method_name( std::move( other.method_name ) )
    {
        //qDebug() << "tagmethod move1";
    }
};


template <>
struct TagMethod<TagMethodType::CompareValue> : TagMethod<TagMethodType::Simple>
{
    QVariant value;
    TagMethod<TagMethodType::CompareValue>() {}
    TagMethod<TagMethodType::CompareValue>(const QString& tn, const QString& mn) : TagMethod<TagMethodType::Simple>(tn, mn) {}
    TagMethod( TagMethod&& other ) : value( std::move(other.value) )
    {
        //qDebug() << "tagmethod move2";
    }
    TagMethod( const TagMethod& ) = default;
};


template <>
struct TagMethod<TagMethodType::Property>
{
    QString tag_name;

    TagMethod(const QString &tn) : tag_name(tn) {}
    TagMethod(){}
    TagMethod( TagMethod&& other ):tag_name( std::move(other.tag_name) )
    {
        //qDebug() << "tagmethod move3";
    }
    TagMethod( const TagMethod& ) = default;
};

#endif // TAGMETHOD_H
