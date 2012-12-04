#include "baseweighter.h"
#include "warnmessages.h"
#include "mainsequence.h"
#include "codeshacks.h"
#include "async_func.h"
#include "genericdbstructs.h"

BaseWeighter::BaseWeighter(MainSequence & as, QSqlDatabase& db) : seq(as),
    database(db), async_(database, as), convience_(async_) {}


void BaseWeighter::checkBruttoDeltaForReweights(int prev_weight, int weight) const throw(MainSequenceException)
{
    if (qAbs( prev_weight - weight ) > seq.appSetting<int>("brutto_delta_between_reweights")) {
        throw MainSequenceException(tr(brutto_rupture_failed_message),
                                "reweight: rup failed!: prevWeight: " +
                                QString::number( prev_weight ) +
                                " cur: " + QString::number(weight)  + " max_delta: " +
                                seq.appSetting<int>("brutto_delta_between_reweights"));

    }
}

void BaseWeighter::checkTaraDeltaForReweights(int prev_weight, int weight) const throw(MainSequenceException)
{
    if (qAbs( prev_weight - weight ) > seq.appSetting<int>("tara_delta_between_reweights")) {
        throw MainSequenceException(tr(tara_rupture_failed_message),
                                "reweight: rup failed!: prevWeight: " +
                                QString::number( prev_weight ) +
                                " cur: " + QString::number(weight)  + " max_delta: " +
                                seq.appSetting<int>("tara_delta_between_reweights"));

    }
}


QString BaseWeighter::platformType(const MifareCardData & bill,
                            const WeighterConf& weighter_conf) const throw (MainSequenceException)
{
    if ( weighter_conf.platform_type != "auto" )
        return weighter_conf.platform_type;

    return detectPlatformType(bill);
}


void BaseWeighter::checkWeightCorrectess(int w)  throw (MainSequenceException)
{
    if ( w < 0 || !seq.alhoSettings().weight_stable.func().toBool() )
        throw MainSequenceException(tr(weight_not_stable_message),
                                    "weights dont stable!" );
}

int BaseWeighter::getWeight()  throw (MainSequenceException)
{
    QVariant v = seq.alhoSettings().weight_tag.func( );

    if (!v.isValid())
        throw MainSequenceException(tr(weights_dont_work), "weights dont work!");
    // 
    //int min_weight = 300;
    //if ( v.toInt() < min_weight)
    //    throw MainSequenceException(tr(weights_dont_work), "weights dont work!");

    return v.toInt();
}

void BaseWeighter::setCardBanned(const QString & uid) throw()
{   
    //asyncFunc() -> setShowDebugInfo(true);

 /*   qx::dao::ptr<bad_cards> bk(new bad_cards);
    bk->uid = uid;
    wrap_async([this, &bk]{
               asyncFunc()->async_insert(bk, false, "bad_cards");
    }); */
}


void BaseWeighter::checkCardBanned(const QString & uid) throw (MainSequenceException)
{

    /*qx::dao::ptr<bad_cards> bk;

    bk = wrap_async([this, &bk, &uid]{return asyncFunc()->async_exec_query<bad_cards>(
                 "select * from bad_cards where uid='" + uid + "'", false);});


    if (bk) {
        throw MainSequenceException (tr(your_card_is_banned), "card :" + uid + " is in banned");
    }*/
}

bool BaseWeighter::transaction()
{
    return asyncFunc()->async_call( [this]{return database.transaction();} );
}

bool BaseWeighter::commit()
{
    return asyncFunc()->async_call( [this]{return database.commit();} );
}

bool BaseWeighter::rollback()
{
    return asyncFunc()->async_call( [this]{return database.rollback();} );
}
