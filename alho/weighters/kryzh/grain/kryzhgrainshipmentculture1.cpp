#include "rdb_pch.h"
#include "kryzhgrainshipmentculture.h"
#include "kryzhgraindbstructs.h"
#include "codeshacks.h"
#include "reportsmanager2.h"

#include <fusion_tools/make_vvector.h>

#include <boost/fusion/include/vector_tie.hpp>

#include <mpllibs/metaparse/string.hpp>

#define _S(X) MPLLIBS_STRING(X)

#ifdef AAAA

namespace alho { namespace kryzh {


void GrainShipmentCulture::brutto(int w, MifareCardData& bill)
{
#if 0
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
               [&bill, this]{return asyncFunc().async_fetch<t_ttno>(
                   bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "GrainShipment: brutto weight!, ttn: " << current_ttn->num_nakl;

    current_ttn->brutto           = w;
    current_ttn->dt_of_brutto     = QDateTime::currentDateTime();

    checkBruttoByTara(w, current_ttn);

    current_prikaz = fetchPrikaz(current_ttn->prikaz);

    checkPrikazClosed(current_prikaz);
    checkPrikazLimit( current_prikaz );


    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    current_prikaz->rest         += current_ttn->netto();

    qDebug() << "virtual rest: 2 " << current_prikaz->virtual_rest;

    updateBruttoValues(bill, current_ttn);
    updatePrikaz(current_prikaz);
#endif
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );

    seq().seqDebug() << "GrainShipment: brutto weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    checkBruttoByTara2(w, ttn_table, current_ttn);

    fetchPrikaz(current_ttn[ttn_table.prikaz]);

    checkPrikazClosed();

    checkPrikazLimit();

    current_prikaz[ prikaz_table.rest ] += ((current_ttn[ttn_table.brutto] - current_ttn[ttn_table.tara]));

    updateBruttoValues(bill);

    updateRest();
}

} }

#undef _S

#endif
