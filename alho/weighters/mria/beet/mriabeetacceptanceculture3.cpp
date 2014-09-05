#include "rdb_pch.h"

#include "mriabeetacceptanceculture.h"
#include "mainsequence.h"
#include "codeshacks.h"

#include <fusion_tools/as_vvector.h>
#include <fusion_tools/make_vvector.h>

#include <boost/fusion/include/vector_tie.hpp>

#ifdef AAAA

namespace alho  { namespace mria {


void BeetAcceptanceCulture::tara(int w, MifareCardData& bill)
{
#if 0
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                              [&bill, this]{return asyncFunc().async_fetch<t_ttn_beet_mria>(bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "BeetAcceptance: tara weight!, ttn: " << current_ttn->num_nakl;

    checkTaraByBrutto(w, current_ttn);

    current_ttn->tara = w;
    current_ttn->dt_of_tara = QDateTime::currentDateTime();

    checkBum(bill);

    clearBumQueue(current_ttn);

    processDrivingTime(current_ttn, current_car);

    

    processTaraRupture<t_ttn_beet_mria, t_cars_beet_mria, t_const_beet_mria>(current_ttn, current_car);

    updateTaraValues(bill, current_ttn, current_car, true);

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());
#endif
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );

    seq().seqDebug() << "BeetAcceptance: tara weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    checkTaraByBrutto2(w, ttn_table, current_ttn);

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());

    checkBum(bill);

    //clearBumQueue();

    processDrivingTime2(ttn_table, current_ttn, cars_table, current_car);

    processTaraRupture2(ttn_table, current_ttn, cars_table, current_car);

    updateTaraValues(bill, boost::mpl::true_{});
}


} }

#endif
