#ifndef CONVIENCEFUNCS_H
#define CONVIENCEFUNCS_H

#include <QSqlDatabase>

#include "warnmessages.h"
#include "async_func.h"
#include "dbstructs.h"
#include "bossnexception.h"

struct convience_func
{
    async_func & async_func_;
    convience_func(async_func& af) : async_func_(af) {}

    inline qx::dao::ptr<t_const> getConst(const QString & k) throw(MainSequenceException)
    {
        auto const_ = async_func_.wrap_async_ex( cant_get_const_message + ": " + k, "cant get const " + k,
                                [&k,this]{return async_func_.async_fetch<t_const>(k);});
        return const_;
    }
};


#endif // CONVIENCEFUNCS_H
