#ifndef TAGMETHOD_H
#define TAGMETHOD_H


#include <QString>
#include <QVariant>

template <bool WithCompareValue>
struct TagMethod
{
    QString tag_name;
    QString method_name;
    TagMethod(const QString& tn, const QString & mn):tag_name(tn), method_name(mn){}
    TagMethod(const QString &tn) : tag_name(tn) {}
    TagMethod(){}
};

template <>
struct TagMethod<true> : TagMethod<false>
{
    QVariant value;
    TagMethod<true>() {}
    TagMethod<true>(const QString& tn, const QString& mn) : TagMethod<false>(tn, mn) {}
};

#endif // TAGMETHOD_H
