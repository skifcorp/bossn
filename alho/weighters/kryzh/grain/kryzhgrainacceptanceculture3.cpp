
#include "rdb_pch.h"

#include "kryzhgrainacceptanceculture.h"
#include "warnmessages.h"
#include "mainsequence.h"
#include "codeshacks.h"
#include "reportsmanager2.h"

#include <fusion_tools/make_vvector.h>

#include <boost/fusion/include/vector_tie.hpp>

#ifdef AAAA

namespace alho { namespace kryzh {

void GrainAcceptanceCulture::reBrutto(int w, MifareCardData& bill)
{

    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );


    seq().seqDebug() << "GrainAcceptance: rebrutto weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    checkBruttoDeltaForReweights(bill.memberValue<uint>("bruttoWeight"), w);

    bill.setMemberValue("flags", 0, true );
    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    updateBruttoValues(bill);
}

}}
#endif
