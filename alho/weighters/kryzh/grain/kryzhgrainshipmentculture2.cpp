
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

void GrainShipmentCulture::reBrutto(int w, MifareCardData& bill)
{
#if 0
    current_ttn = ttnByDriver<t_ttno>( bill.memberValue<uint>("driver") );

    seq().seqDebug() << "GrainShipment: retara weight!, ttn: " << current_ttn->num_nakl;



    checkBruttoByTara(w, current_ttn);

    checkBruttoDeltaForReweights(current_ttn->brutto, w);

    current_prikaz =  fetchPrikaz(current_ttn->prikaz);
    checkPrikazClosed( current_prikaz );
    checkPrikazLimit( current_prikaz );

    rollbackPrikaz( current_prikaz, current_ttn );

    current_ttn->brutto = w;
    current_ttn->dt_of_brutto = QDateTime::currentDateTime();

    current_prikaz->rest += current_ttn->netto();

    //qx::dao::ptr<t_ttno> ttn_max_netto = ttnWithMaxNetto();
    //if (ttn_max_netto)
    //   current_prikaz->virtual_rest -= ttn_max_netto->netto();

    //current_prikaz->virtual_rest += current_ttn->netto();


    updateBruttoValues(bill, current_ttn);
    updatePrikaz(current_prikaz);
#endif
    current_ttn = ttnByDriver2< decltype(current_ttn) >(  bill.memberValue<int>("driver"), ttn_table );

    seq().seqDebug() << "GrainShipment: retara weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    checkBruttoByTara2(w, ttn_table, current_ttn);

    checkBruttoDeltaForReweights(current_ttn[ttn_table.brutto], w);

    fetchPrikaz(current_ttn[ttn_table.prikaz]);

    checkPrikazClosed();

    checkPrikazLimit();

    rollbackPrikaz( );

    current_prikaz[ prikaz_table.rest ] += ((current_ttn[ttn_table.brutto] - current_ttn[ttn_table.tara]));

    updateBruttoValues(bill);

    updateRest();
}

}}

#endif
