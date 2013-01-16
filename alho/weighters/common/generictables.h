#ifndef GENERICTABLES_H
#define GENERICTABLES_H

#include  "qxorm_pch.h"


BOOST_RDB_BEGIN_TABLE(t_const_table)
    BOOST_RDB_COLUMN(id                           ,varchar<64>)
    BOOST_RDB_COLUMN(value                        ,varchar<256>)
BOOST_RDB_END_TABLE(t_const_table)


#endif // GENERICTABLES_H
