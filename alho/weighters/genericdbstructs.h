#ifndef GENERICDBSTRUCTS_H
#define GENERICDBSTRUCTS_H

#include "qxorm_pch.h"

struct bad_cards
{
    long id;
    QString uid;

    bad_cards(long i) : id(i) {}
    bad_cards() : id(-1) {}

    virtual ~bad_cards(){}
};


QX_REGISTER_HPP_EXPORT_DLL(bad_cards          , qx::trait::no_base_class_defined, 1)

#endif // GENERICDBSTRUCTS_H
