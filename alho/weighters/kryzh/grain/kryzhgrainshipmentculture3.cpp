#ifdef AAAA

#include "rdb_pch.h"
#include "kryzhgrainshipmentculture.h"
#include "kryzhgraindbstructs.h"
#include "codeshacks.h"
#include "reportsmanager2.h"

#include <fusion_tools/make_vvector.h>

#include <boost/fusion/include/vector_tie.hpp>

#include <mpllibs/metaparse/string.hpp>

#define _S(X) MPLLIBS_STRING(X)



namespace alho { namespace kryzh {

void GrainShipmentCulture::tara(int w, MifareCardData& bill)
{
#if 0
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                              [&bill, this]{return asyncFunc().async_fetch<t_ttno>(
                  bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "GrainShipment: tara weight!, ttn: " << current_ttn->num_nakl;

    current_prikaz = fetchPrikaz(current_ttn->prikaz);

    checkPrikazClosed( current_prikaz );

    //current_prikaz->virtual_rest += getMaxNetto(current_prikaz);

    checkVirtualNetto(current_prikaz);

    current_ttn->tara = w;
    current_ttn->dt_of_tara = QDateTime::currentDateTime();

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());

    //processTaraRupture<t_ttno, t_cars, t_const>(current_ttn, current_car);

    updateTaraValues(bill, current_ttn);
    //updatePrikaz(current_prikaz);
#endif
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );

    seq().seqDebug() << "GrainShipment: tara weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    fetchPrikaz(current_ttn[ttn_table.prikaz]);

    checkPrikazClosed( );

    checkVirtualNetto( );

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());

    updateTaraValues(bill);
    updateVirtualRest();
}

}}

#endif
