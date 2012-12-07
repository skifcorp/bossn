#include "culture.h"
#include "warnmessages.h"
#include "mainsequence.h"
#include "codeshacks.h"
#include "async_func.h"
#include "genericdbstructs.h"


namespace alho { namespace common {

Culture::Culture(MainSequence & as, QSqlDatabase& db) : seq_(as),
    database_(db), async_(database_, as), convience_(async_) {}


void Culture::checkBruttoDeltaForReweights(int prev_weight, int weight) const throw(MainSequenceException)
{
    if (qAbs( prev_weight - weight ) > seq().appSetting<int>("brutto_delta_between_reweights")) {
        throw MainSequenceException(tr(brutto_rupture_failed_message),
                                "reweight: rup failed!: prevWeight: " +
                                QString::number( prev_weight ) +
                                " cur: " + QString::number(weight)  + " max_delta: " +
                                seq().appSetting<int>("brutto_delta_between_reweights"));

    }
}

void Culture::checkTaraDeltaForReweights(int prev_weight, int weight) const throw(MainSequenceException)
{
    if (qAbs( prev_weight - weight ) > seq().appSetting<int>("tara_delta_between_reweights")) {
        throw MainSequenceException(tr(tara_rupture_failed_message),
                                "reweight: rup failed!: prevWeight: " +
                                QString::number( prev_weight ) +
                                " cur: " + QString::number(weight)  + " max_delta: " +
                                seq().appSetting<int>("tara_delta_between_reweights"));

    }
}


void Culture::checkWeightCorrectess(int w)  throw (MainSequenceException)
{
    if ( w < 0 || !seq().alhoSettings().weight_stable.func().toBool() )
        throw MainSequenceException(tr(weight_not_stable_message),
                                    "weights dont stable!" );
}

int Culture::getWeight()  throw (MainSequenceException)
{
    QVariant v = seq().alhoSettings().weight_tag.func( );

    if (!v.isValid())
        throw MainSequenceException(tr(weights_dont_work), "weights dont work!");
    // 
    //int min_weight = 300;
    //if ( v.toInt() < min_weight)
    //    throw MainSequenceException(tr(weights_dont_work), "weights dont work!");

    return v.toInt();
}

} }
