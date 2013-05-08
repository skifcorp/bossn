#include "rdb_pch.h"
#include "kryzhbeetacceptanceculture.h"
#include "mainsequence.h"
#include "codeshacks.h"
#include "kryzhbeettables.h"
#include "reportsmanager2.h"

#include "fusion_tools/make_vvector.h"

#include <boost/fusion/include/vector_tie.hpp>



namespace alho  { namespace kryzh {

QString BeetAcceptanceCulture::bruttoFinishMessage(const MifareCardData& bill )const
{
    QString ret;
    ret = tr(brutto_finish_weight_message).arg( bill.memberValue<QString>("bruttoWeight") );
    if ( bill.memberValue<QBitArray>("flags").at(2) ) {
        ret += " " +  tr(brutto_finish_lab_message).arg( bill.memberValue<QString>("pointOfAnal") );
    }
    ret += " " + tr(brutto_finish_bum_message).arg( bill.memberValue<QString>("bum") );

    return ret;

}

QString BeetAcceptanceCulture::taraFinishMessage(const MifareCardData& )const
{
    QString ret;
    //ret = tr(brutto_finish_weight_message).arg( current_ttn->tara );
    ret = tr(brutto_finish_weight_message).arg( current_ttn[ttn_table.tara] );
    return ret;
}




void BeetAcceptanceCulture::brutto(int w, MifareCardData& bill)
{


    /*current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                           [&bill, this]{return asyncFunc().async_fetch<t_ttn_beet>(bill["billNumber"].toUInt(), t_ttn_name);});
*/
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );

    //seq().seqDebug() << "BeetAcceptance: brutto weight!, ttn: " << current_ttn->num_nakl;

    seq().seqDebug() << "BeetAcceptance: brutto weight!, ttn: " << current_ttn[ ttn_table.num_nakl ];

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    repairFieldCodeCorrectnessIfNeeded2(bill, field_table, ttn_table, current_ttn);

    processChemicalAnalysis( bill );
    processFreeBum( bill ); 

    updateBruttoValues(bill);
}







} }
