#ifndef TAGMETHOD_H
#define TAGMETHOD_H


#include <QString>

struct TagMethod
{
    QString tag_name;
    QString method_name;
    TagMethod(const QString& tn, const QString & mn):tag_name(tn), method_name(mn){}
    TagMethod(const QString &tn) : tag_name(tn) {}
    TagMethod(){}
};

#endif // TAGMETHOD_H
