#include "rdb_pch.h"
#include "kryzhbeetacceptanceculture.h"
#include "mainsequence.h"
#include "codeshacks.h"
#include "kryzhbeettables.h"
#include "reportsmanager2.h"

#include "fusion_tools/make_vvector.h"

#include <boost/fusion/include/vector_tie.hpp>

#ifdef AAAA

namespace alho  { namespace kryzh {


void BeetAcceptanceCulture::tara(int w, MifareCardData& bill)
{
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );

    seq().seqDebug() << "BeetAcceptance: tara weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    checkTaraByBrutto2(w, ttn_table, current_ttn);

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());

    checkBum(bill);
    checkLaboratory(bill);
    checkKagat(bill);

    clearBumQueue();

    processDrivingTime2(ttn_table, current_ttn, cars_table, current_car  );

    repairBumCorrectnessIfNeeded();

    processTaraRupture2(ttn_table, current_ttn, cars_table, current_car);

    updateTaraValues(bill, boost::mpl::true_{});


}

} }

#endif
