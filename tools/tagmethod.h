#ifndef TAGMETHOD_H
#define TAGMETHOD_H


#include <QString>
#include <QVariant>

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
};


template <>
struct TagMethod<TagMethodType::CompareValue> : TagMethod<TagMethodType::Simple>
{
    QVariant value;
    TagMethod<TagMethodType::CompareValue>() {}
    TagMethod<TagMethodType::CompareValue>(const QString& tn, const QString& mn) : TagMethod<TagMethodType::Simple>(tn, mn) {}
};


template <>
struct TagMethod<TagMethodType::Property>
{
    QString tag_name;

    TagMethod(const QString &tn) : tag_name(tn) {}
    TagMethod(){}
};

#endif // TAGMETHOD_H
