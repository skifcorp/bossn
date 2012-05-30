#ifndef CONVIENCEFUNCS_H
#define CONVIENCEFUNCS_H

#include "warnmessages.h"
#include "async_func.h"
#include "dbstructs.h"
#include "bossnexception.h"

inline qx::dao::ptr<t_const> getConst(const QString & k) throw(MainSequenceException)
{
    auto const_ = wrap_async_ex( cant_get_const_message + ": " + k, "cant get const " + k,
                                [&k]{return async_fetch<t_const>(k);});
    return const_;
}


#endif // CONVIENCEFUNCS_H
