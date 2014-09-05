#ifdef AAAA

#include "rdb_pch.h"

#include "kryzhgrainacceptanceculture.h"
#include "warnmessages.h"
#include "mainsequence.h"
#include "codeshacks.h"
#include "reportsmanager2.h"

#include <fusion_tools/make_vvector.h>

#include <boost/fusion/include/vector_tie.hpp>



namespace alho { namespace kryzh {


void GrainAcceptanceCulture::brutto(int w, MifareCardData& bill )
{
#if 0
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
               [&bill, this]{return asyncFunc().async_fetch<t_ttn>(
                   bill["billNumber"].toUInt(), t_ttn_name);});
#endif
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );


    seq().seqDebug() << "GrainAcceptance: brutto weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    repairFieldCodeCorrectnessIfNeeded2(bill, field_table, ttn_table, current_ttn);

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());
    bill.setMemberValue("flags", 2, true );

    current_ttn[ttn_table.routed_to_lab] = 1;

    current_ttn[ttn_table.culture] = bill.memberValue<int>("culture");
    current_ttn[ttn_table.repr]    = bill.memberValue<int>("repr");
    current_ttn[ttn_table.sort]    = bill.memberValue<int>("sort");

    updateBruttoValues(bill);
}


}}
#endif
