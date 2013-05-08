#include "rdb_pch.h"

#include "mriabeetacceptanceculture.h"
#include "mainsequence.h"
#include "codeshacks.h"

#include <fusion_tools/as_vvector.h>
#include <fusion_tools/make_vvector.h>

#include <boost/fusion/include/vector_tie.hpp>

namespace alho  { namespace mria {



void BeetAcceptanceCulture::brutto(int w, MifareCardData& bill)
{

#if 0
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                           [&bill, this]{return asyncFunc().async_fetch<t_ttn_beet_mria>(bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "BeetAcceptance: brutto weight!, ttn: " << current_ttn->num_nakl;

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    repairFieldCodeCorrectnessIfNeeded<t_ttn_beet_mria, t_field_beet_mria>(bill, current_ttn, t_field_name);

    updateBruttoValues(bill, current_ttn);
#endif
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );

    seq().seqDebug() << "BeetAcceptance: brutto weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    repairFieldCodeCorrectnessIfNeeded2(bill, field_table, ttn_table, current_ttn);

    updateBruttoValues(bill);
}



} }
