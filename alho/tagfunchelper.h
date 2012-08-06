#ifndef TAGFUNCHELPER_H
#define TAGFUNCHELPER_H

#include "tags.h"
#include "alhosequence.h"




template <bool, class T1, class T2>
struct TypeSelect
{
    typedef T1 result;
};

template <class T1, class T2>
struct TypeSelect<false, T1, T2>
{
    typedef T2 result;
};


template <bool IsRef>
struct TagFuncHelper
{
public:
    TagFuncHelper(const QString& tn, const QString& fn, AlhoSequence& s, Tags & t ) :
        func_name(fn), tag_name_ref( tn ), seq(s), tags(t)
    {

    }
    TagFuncHelper(const QString& tn, AlhoSequence& s, Tags & t ) :
        tag_name_ref( tn ), seq(s), tags(t)
    {

    }

    QVariant func (QGenericArgument val0 = QGenericArgument(),
                   QGenericArgument val1 = QGenericArgument(),
                   QGenericArgument val2 = QGenericArgument(),
                   QGenericArgument val3 = QGenericArgument(),
                   QGenericArgument val4 = QGenericArgument(),
                   QGenericArgument val5 = QGenericArgument())
    {
        return tags[tag_name_ref]->func(func_name, &seq, val0, val1, val2, val3, val4, val5);
    }
    typename TypeSelect<IsRef, const QString&, QString>::result func_name;
private:
    const QString& tag_name_ref;

    AlhoSequence & seq;
    Tags & tags;
};

#endif // TAGFUNCHELPER_H
