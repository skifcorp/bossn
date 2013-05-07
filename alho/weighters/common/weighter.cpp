#include "rdb_pch.h"
#include "weighter.h"



namespace alho { namespace common {

void Weighter::setCardBanned(const QString & )
{   
    //asyncFunc() -> setShowDebugInfo(true);

 /*   qx::dao::ptr<bad_cards> bk(new bad_cards);
    bk->uid = uid;
    wrap_async([this, &bk]{
               asyncFunc()->async_insert(bk, false, "bad_cards");
    }); */
}


void Weighter::checkCardBanned(const QString & )
{

    /*qx::dao::ptr<bad_cards> bk;

    bk = wrap_async([this, &bk, &uid]{return asyncFunc()->async_exec_query<bad_cards>(
                 "select * from bad_cards where uid='" + uid + "'", false);});


    if (bk) {
        throw MainSequenceException (tr(your_card_is_banned), "card :" + uid + " is in banned");
    }*/
}


} }
