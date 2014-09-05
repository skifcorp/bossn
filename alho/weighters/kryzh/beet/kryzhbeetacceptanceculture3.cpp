#include "rdb_pch.h"
#include "kryzhbeetacceptanceculture.h"
//#include "mainsequence.h"
//#include "codeshacks.h"
#include "kryzhbeettables.h"
//#include "reportsmanager2.h"

//#include "fusion_tools/make_vvector.h"

//#include <boost/fusion/include/vector_tie.hpp>

#ifdef AAAA

namespace alho  { namespace kryzh {



void BeetAcceptanceCulture::reTara(int w, MifareCardData& bill)
{
    qDebug() << "ttn_by_driver";

    current_ttn = ttnByDriver2< decltype(current_ttn) >(  bill.memberValue<int>("driver"), ttn_table );

    seq().seqDebug() << "BeetAcceptance: retara weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    checkTaraByBrutto2(w, ttn_table, current_ttn);

    checkTaraDeltaForReweights(current_ttn[ttn_table.tara], w);

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());

    repairBumCorrectnessIfNeeded();

    processDrivingTime2(ttn_table, current_ttn, cars_table, current_car);

    processTaraRupture2(ttn_table, current_ttn, cars_table, current_car);

    updateTaraValues(bill, boost::mpl::false_{});
}



} }

#endif
