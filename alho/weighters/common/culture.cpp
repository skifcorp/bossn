#include "culture.h"
#include "warnmessages.h"
#include "mainsequence.h"
#include "codeshacks.h"
#include "async_func.h"
#include "genericdbstructs.h"
#include "func.h"

#include "generictables.h"

#include <boost/fusion/include/at_c.hpp>

#include <QTextCodec>

namespace alho { namespace common {

Culture::Culture(MainSequence & as, QSqlDatabase& db, boost::rdb::mysql::mysql_database& db2) : seq_(as),
    database_(db), async_(database_, as), convience_(async_), async2_(db2, as) {}


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
    if ( w < 0 ) {
        throw MainSequenceException(tr(weight_not_stable_message),
                                    "weights dont stable!: " + QString::number(w) );
    }

    if (  !seq().alhoSettings().weight_stable.func().toBool() )
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

namespace sql = boost::rdb::mysql;

void Culture::fillConstants()
{
    t_const_table t_const{"t_const"};
    typename sql::table_result_set<t_const_table>::type deq;

    deq = async2().exec( sql::select( t_const.all ).from( t_const ), cant_get_const_message ).all();

    constants_.clear();

    for ( const auto & val : deq ) {
        constants_[  QString::fromUtf8( val[t_const.id].c_str() ) ] =
                 QString::fromUtf8( val[t_const.value].c_str() );
    }
}

} }
