#include "kryzhbeetacceptanceculture.h"
#include "mainsequence.h"
#include "codeshacks.h"
#include "kryzhbeettables.h"

#include "fusion_tools/make_vvector.h"

namespace alho  { namespace kryzh {

/*const QString BeetAcceptanceCulture::t_cars_name("t_cars");
const QString BeetAcceptanceCulture::t_ttn_name("t_ttn");
const QString BeetAcceptanceCulture::t_const_name("t_const");
const QString BeetAcceptanceCulture::t_kontr_name("t_kontr");
const QString BeetAcceptanceCulture::t_bum_name("t_bum");
const QString BeetAcceptanceCulture::t_kagat_name("t_kagat");
const QString BeetAcceptanceCulture::t_field_name("t_field");*/


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

//    if ( bill.memberValue<QBitArray>("flags").at(2) )
//        bill.setMemberValue("flags", 3, true);
//    else
//        bill.setMemberValue("flags", 3, false);

    //current_ttn->sort = bill.memberValue<int>("sort");
    //current_ttn->culture = bill.memberValue<int>("culture");
    //current_ttn->repr = bill.memberValue<int>("repr");    

    updateBruttoValues(bill);
}

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

void BeetAcceptanceCulture::reBrutto(int w, MifareCardData& bill)
{
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );

    seq().seqDebug() << "BeetAcceptance: rebrutto weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    checkBruttoDeltaForReweights(bill.memberValue<uint>("bruttoWeight"), w);

    bill.setMemberValue("flags", 0, true );
    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

/*    if ( bill.memberValue<QBitArray>("flags").at(2) ) {
        bill.setMemberValue("flags", 3, true);
    }
    else {
        bill.setMemberValue("flags", 3, false);
    }*/

    updateBruttoValues(bill);
}

void BeetAcceptanceCulture::reTara(int w, MifareCardData& bill)
{
    qDebug() << "ttn_by_driver";

    current_ttn = ttnByDriver2< decltype(current_ttn) >(  bill.memberValue<int>("driver"), ttn_table );

    seq().seqDebug() << "BeetAcceptance: retara weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    checkTaraByBrutto2(w, ttn_table, current_ttn);

    checkTaraDeltaForReweights(current_ttn[ttn_table.tara], w);

    //current_ttn[ttn_table.tara] = w;
    //current_ttn[ttn_table.dt_of_tara] = boost::posix_time::from_time_t( QDateTime::currentDateTime().toTime_t() );

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());

    repairBumCorrectnessIfNeeded();

    processDrivingTime2(ttn_table, current_ttn, cars_table, current_car);

    processTaraRupture2(ttn_table, current_ttn, cars_table, current_car);

    updateTaraValues(bill, boost::mpl::false_{});
}


uint BeetAcceptanceCulture::countCarsFromFieldForDayExcludeCurrent(int field_num)
{
    try {
        QDate workDate = QTime::currentTime().hour()<8 ? QDate::currentDate().addDays(-1) : QDate::currentDate();

/*        qx_query q("where floor(real_field/100) = "  + QString::number( field_num ) +
                   " and dt_of_brutto>='" + workDate.toString("yyyy.MM.dd") + " 08:00:00'"                            \
                   " and dt_of_brutto<='" + workDate.addDays(1).toString("yyyy.MM.dd") + " 08:00:00' and num_nakl <> " +
                   QString::number(ttn_num) );
        long count = 0;
        asyncFunc().async_count<t_ttn_beet>( count, q, t_ttn_name);

        return count;*/
        int ttn_num = current_ttn[ ttn_table.num_nakl ];
        auto q = sql::select( sql::count() ).from( ttn_table ).where ( sql::floor( ttn_table.real_field / 100 ) == field_num &&
            ttn_table.dt_of_brutto >= (workDate.toString("yyyy.MM.dd") + " 08:00:00").toAscii().constData() &&
            ttn_table.dt_of_brutto <= (workDate.addDays(1).toString("yyyy.MM.dd") + " 08:00:00").toAscii().constData() &&
            ttn_table.num_nakl != ttn_num );

        return async2().fetch( q, "counting cars failed!" );
    }
    catch (MainSequenceException & ex) {
        seq().seqWarning() << "cant get count of ttns for chemical analysis!!! field: "<<field_num << " msg: " << ex.adminMessage();
    }

    return 0;
}

bool BeetAcceptanceCulture::analysisEnabled()
{
/*    qx::dao::ptr<t_const_beet> enab_analysis;
    try {
        enab_analysis = convienceFunc().getConst<t_const_beet>(seq().appSetting<QString>("enable_analysis", "enable_analysis"));
    }
    catch (MainSequenceException& ex) {
        qDebug () << "cant get enable_analysis!!!!";

        return true;
    }

    if (!enab_analysis) {
        qDebug() << "enable_analysis is NULL!!";
        return true; //this must always be true here
    }

    bool ok = false;

    int ret = enab_analysis->value.toInt(&ok);

    if(!ok) {
        qDebug() << "cant convert enable_analysis to int!!!";
        return true;
    }

    qDebug () << "enable_analysis : " << ret;*/

    if (!containsConstant(seq().appSetting<QString>("enable_analysis", "enable_analysis")))
        return true;

    return constantValue<bool>( seq().appSetting<QString>("enable_analysis", "enable_analysis"));
}

void BeetAcceptanceCulture::processChemicalAnalysis(MifareCardData & bill )
{
    if ( !analysisEnabled() ) return;

    long count   = countCarsFromFieldForDayExcludeCurrent( kontrCodeFromField( bill.memberValue<uint>("realNumField") ) );

    QString alho = seq().appSetting<QString>("common_algorithm_of_analysis");

    if ( alho == "discrete"  ) {
        if ( !checkForNeedDiscreteAnalisys(count) ) {
            return;
        }
    }
    else if (alho == "database_const") {
        if ( !checkForNeedDatabaseConstAnalisys( count,
                                 kontrCodeFromField( bill.memberValue<uint>("realNumField") ) ) ) {
            return;
        }
    }

    uint analysis_place = (count+1)%3;

    if (analysis_place==0) analysis_place=3;

    bill.setMemberValue("flags", 2, true );
    bill.setMemberValue("pointOfAnal", analysis_place);
}


bool BeetAcceptanceCulture::checkForNeedDiscreteAnalisys(long count) const
{
    uint num_in_group = (count + 1) % seq().appSetting<uint>("common_size_of_group");
    return num_in_group == seq().appSetting<uint>("common_number_from_group");
}

uint BeetAcceptanceCulture::getAnalisysPeriodFromStorage(uint typ)
{
#if 0
    qx::dao::ptr<t_const_beet> const_;

    if ( typ == 0 ) {
        const_ = wrap_async_ex( "error for kontragent type when doint chemical analysis typ=0", QString(),
            [this]{return asyncFunc().async_fetch<t_const_beet>(seq().appSetting<QString>("corpotare_check_period_name"), t_const_name);});
    }
    else if ( typ == 3 ) {
        const_ = wrap_async_ex( "error for kontragent type when doint chemical analysis typ=3", QString(),
            [this]{return asyncFunc().async_fetch<t_const_beet>(seq().appSetting<QString>("farmer_check_period_name"), t_const_name); });
    }
    else {
        throw MainSequenceException("error for kontragent type when doint chemical analysis", QString());
    }
#endif

    int ret{};

    if ( typ == 0 ) {
        ret = constantValue<int>( seq().appSetting<QString>("corpotare_check_period_name") );
    }
    else if ( typ == 1 ) {
        ret = constantValue<int>( seq().appSetting<QString>("farmer_check_period_name") );
    }
    else {
        throw MainSequenceException("error for kontragent type when doint chemical analysis", QString());
    }

    return ret;
}

bool BeetAcceptanceCulture::checkForNeedDatabaseConstAnalisys(long count, long kontrag)
{
    if (count + 1 == 1) return true; //first car always go for analysis

    try {
#if 0
        auto kontr = wrap_async_ex( "cant get kontr of ttns for chemical analysis!!! kontrag: " + QString::number(kontrag),
                                     QString(),
                [&kontrag, this]{return asyncFunc().async_fetch<t_kontr_beet>(kontrag, t_kontr_name);} );
#endif

        auto kontr = async2().fetch(
                        sql::select( kontr_table.period,
                                     kontr_table.car_in_period,
                                     kontr_table.type ).from( kontr_table ).where( kontr_table.id == kontrag ),
                        "cant get kontr of ttns for chemical analysis!!! kontrag: " + QString::number(kontrag)
                     );

        uint period = kontr[kontr_table.period];

        if ( period == 0 ) {
            period = getAnalisysPeriodFromStorage(kontr[kontr_table.type]);
        }

        uint carInPeriod = kontr[kontr_table.car_in_period];
        if (carInPeriod == 0 ) {
            carInPeriod = 1;
            seq().seqWarning() << "error for getting carInPeriod. Kontragent dont have corrent chemical analisys params";
        }

        if ( period == 0 )  {
            period = 1;
            seq().seqWarning() << "error for getting period. Kontragent dont have corrent chemical analisys params";
        }

        uint num_in_group = (count + 1)%period;
        if (num_in_group == 0) num_in_group = period;
        return num_in_group == carInPeriod;
    }
    catch (MainSequenceException& ex) {
        seq().seqWarning() << ex.adminMessage();
        return true;
    }
}




void BeetAcceptanceCulture::processFreeBum(MifareCardData & bill)
{
#if 0
    QString bums_where_clause = getBumsClause(bill);

    const QString q1 =  "select * from t_bum where state=1 and queue=(select min(queue) from t_bum where state=1 and ("+bums_where_clause+")) and ("+bums_where_clause +");";
    const QString q2 =  "select * from t_bum where queue=(select min(queue) from t_bum where ("+bums_where_clause+")) and ("+bums_where_clause +");";

    qx::dao::ptr<t_bum_beet> bum = wrap_async_ex(tr(get_free_bum_error),
                "Error getting free bum1: ttn: " + QString::number( current_ttn[ttn_table.num_nakl] ),
                [&bums_where_clause, &q1, this]{ return asyncFunc().async_exec_query<t_bum_beet>(q1, false);});



    if ( !bum ) {
        bum = wrap_async_ex(tr(get_free_bum_error),
                "Error getting free bum2 ttn: " + QString::number(current_ttn[ttn_table.num_nakl]),
                [&bums_where_clause, &q2, this]{ return asyncFunc().async_exec_query<t_bum_beet>(q2);});
    }


    bum->queue += 1;



    wrap_async_ex( tr(update_bum_queue_error),
                   "Error updating bum queue... ttn: " + QString::number(current_ttn[ttn_table.num_nakl]) +
                   "bum: " + QString::number(bum->id)  +
                   " queue: " + QString::number(bum->queue),
                   [&bum, this]{ asyncFunc().async_update(bum, t_bum_name); });

    if ( bum->id != 99 ) {
        current_ttn[ttn_table.bum_platforma] = bum->id % 10;
        current_ttn[ttn_table.bum]           = bum->id / 10;
                //wrap_async_ex( update_ttn_platform_error, "Error updating ttn platform", [&ttn]{ async_update(ttn); });
        bill.setMemberValue("bum", current_ttn[ttn_table.bum]);
     }
     else {
        current_ttn[ttn_table.bum]           = bum->id;

        bill.setMemberValue("bum", bum->id);
     }
#endif
    const int backboard_max = constantValue<int>( seq().appSetting<QString>("bum11_name") );

    const bool dbt = current_car[cars_table.dump_body_truck];
    const bool sb = current_car[cars_table.side_board];
    const bool bbb = current_car[cars_table.back_board] && bill.memberValue<int>("bruttoWeight") > backboard_max;
    const bool bbs = current_car[cars_table.back_board] && bill.memberValue<int>("bruttoWeight") <= backboard_max;



    auto q1 = sql::select(sql::min(bum_table.queue), bum_table.all).from(bum_table).
            where( ((bum_table.id == 99).if_(dbt) || (bum_table.id % 10 == 2).if_(sb) ||
                    (bum_table.id == 11).if_(bbb) || (bum_table.id % 10 == 1).if_(bbs)) && (bum_table.state == true) );



    auto bums = async2().exec(q1, tr(get_free_bum_error)).all();

    if ( bums.size() == 0 ) {
        auto q2 = sql::select(sql::min(bum_table.queue), bum_table.all).from(bum_table).
                where( (bum_table.id == 99).if_(dbt) || (bum_table.id % 10 == 2).if_(sb) ||
                        (bum_table.id == 11).if_(bbb) || (bum_table.id % 10 == 1).if_(bbs));

        bums = async2().exec(q2, tr(get_free_bum_error)).all();
        if ( bums.size( ) == 0  ) {
            throw MainSequenceException(tr(get_free_bum_error),
                      "cant find any bums! car: " +
                      QString::number(current_car[cars_table.id]));
        }
    }

    auto bum = bums.front();

    async2().exec( sql::update(bum_table).set( bum_table.queue = bum_table.queue + 1 )
                   .where( bum_table.id == bum[bum_table.id] ), tr(update_bum_queue_error) );

    if ( bum[bum_table.id] != 99 ) {
        current_ttn[ttn_table.bum_platforma] = bum[bum_table.id] % 10;
        current_ttn[ttn_table.bum]           = bum[bum_table.id] / 10;
                //wrap_async_ex( update_ttn_platform_error, "Error updating ttn platform", [&ttn]{ async_update(ttn); });
        bill.setMemberValue("bum", current_ttn[ttn_table.bum]);
     }
     else {
        current_ttn[ttn_table.bum]           = bum[bum_table.id];

        bill.setMemberValue("bum", bum[bum_table.id]);
     }





    //qFatal("exiting...");
}


QString BeetAcceptanceCulture::getBumsClause(const MifareCardData & bill)
{
    QStringList ret;
#if 0
    if (current_car[cars_table.dump_body_truck]) {
        ret.append("id = 99");
    }

    if (current_car[cars_table.side_board]) {
        ret.append("(id % 10) = 2");
    }

    if (current_car[cars_table.back_board]) {
        int backboard_max = constantValue<int>( seq().appSetting<QString>("bum11_name") );

        if ( bill.memberValue<int>("bruttoWeight") > backboard_max ) {
            ret.append("id = 11");
        }
        else {
            ret.append("(id % 10) = 1");
        }
    }
#endif
    return  ret.join(" or ");
}


void BeetAcceptanceCulture::repairBumCorrectnessIfNeeded()
{
    if ( current_ttn[ttn_table.bum] == current_ttn[ ttn_table.real_bum ] ) return;

    int bum = 99;
    if (current_ttn[ttn_table.bum] != 99)
        bum = current_ttn[ttn_table.bum] * 10 + current_ttn[ttn_table.bum_platforma];

    if ( !checkBumWorks( ptime_to_qt(current_ttn[ttn_table.dt_of_brutto]), ptime_to_qt(current_ttn[ttn_table.dt_of_tara]), bum) ) {
        current_ttn[ttn_table.bum] = current_ttn[ttn_table.real_bum];
    }
}


bool BeetAcceptanceCulture::checkBumWorks(const QDateTime & date_from, const QDateTime & date_to, long bum)
{
#if 0
    const QString q = "select * from t_bum_state_log where date_time >= '" + date_from.toString(rdb_date_time_format) + "' and date_time <= '" + date_to.toString("yyyy.MM.dd hh:mm:ss") + "' and bum = " + QString::number(bum);
    qx::dao::ptr<t_bum_state_log_beet> bum_log = wrap_async_ex( tr(cant_get_bum_state_log_message), "cant get bum state log",
        [&q, this]{ return asyncFunc().async_exec_query<t_bum_state_log_beet>(q, false) ;});



    if (bum_log) return false;

    qx::dao::ptr<t_bum_beet> bum_ptr = wrap_async_ex(tr(cant_get_bum_message), "cant get bum: " + QString::number(bum),
                        [&bum, this]{ return asyncFunc().async_fetch<t_bum_beet>(bum, t_bum_name);});

    return static_cast<bool> (bum_ptr->state);
#endif

    int count = async2().fetch(sql::select(sql::count()).from( t_bum_state_log_table )
            .where( t_bum_state_log_table.date_time >= date_from.toString(rdb_date_time_format).toAscii().constData() &&
                    t_bum_state_log_table.date_time <= date_to.toString(rdb_date_time_format).toAscii().constData() &&
                    t_bum_state_log_table.bum == bum ),
              tr(cant_get_bum_state_log_message) );
    if (count == 0)
        return false;

    auto state = async2().fetch(sql::select(bum_table.state).from(bum_table).where(bum_table.id == bum),
                              tr(cant_get_bum_message));

    //static_name_of<decltype(b)>  asasas;

    return state;
}



void BeetAcceptanceCulture::checkBum( MifareCardData& bill )const
{
    if ( bill.memberValue<int>("bum") == 99 && bill.memberValue<int>("bumFact") == 0 ) {
        bill.setMemberValue("bumFact", 99);
        bill.setMemberValue("kagat", 99);
            //wrap_async_ex( update_bum_queue_error, "Error updating bum queue", [&bum, this]{ async_update(bum); });
    }
}

void BeetAcceptanceCulture::checkLaboratory( const MifareCardData& bill )
{
    if ( bill.memberValue<QBitArray>("flags").at(2) && !bill.memberValue<QBitArray>("flags").at(3) ) {
        current_car[cars_table.block] = 1;

        async2().exec( sql::update(cars_table).set(cars_table.block = true)
                       .where( cars_table.id == current_car[cars_table.id] ),
                        tr(blocking_car_for_lab_error));

        throw MainSequenceException (tr(car_dont_was_in_lab), "car dont was in lab");
    }
}

void BeetAcceptanceCulture::checkKagat(const MifareCardData& bill)
{
    if ( bill.memberValue<int>("kagat") ==0 )
        throw MainSequenceException(tr(car_has_not_been_unloaded), "car was not unloaded!");

#if 0
    qx::dao::ptr<t_kagat_beet> kagat = wrap_async_ex(tr(getting_kagat_error), "cant get kagat: " + bill.memberValue<QString>("kagat"),
       [&bill, this]{ return asyncFunc().async_fetch<t_kagat_beet>(bill.memberValue<int>("kagat"), t_kagat_name) ;});

    if (!kagat->state) {
        throw MainSequenceException(tr(kagat_was_closed_error), "kagat " + bill.memberValue<QString>("kagat") + " was closed!");
    }
#endif
    auto kag = async2().fetch( sql::select( kagat_table.state )
                               .from(kagat_table).where(kagat_table.id == bill.memberValue<int>("kagat")),
                               tr(getting_kagat_error) );

    if ( !kag ) {
        throw MainSequenceException(tr(kagat_was_closed_error), "kagat " + bill.memberValue<QString>("kagat") + " was closed!");
    }
}


void BeetAcceptanceCulture::clearBumQueue()
{
    int bum = current_ttn[ttn_table.bum];

    if (bum != 99) {
        bum = bum * 10 + current_ttn[ttn_table.bum_platforma];
    }

#if 0
    wrap_async_ex(tr(clear_bum_queue_error), "clear bum queue error",
                [&ttn, this, &bum]{ asyncFunc().async_call_query("update t_bum set queue = GREATEST(queue - 1, 0) where id=" + QString::number(bum) + ";") ;});

#endif

    async2().exec( sql::update(bum_table).set( bum_table.queue = sql::greatest( bum_table.queue - 1, 0 ) ).where( bum_table.id == bum ),
                   tr(clear_bum_queue_error));
}

void BeetAcceptanceCulture::updateBruttoValues(MifareCardData& bill)
{
#if 0
    ttn->real_field       = bill.memberValue<int>("realNumField");
    ttn->loader           = bill.memberValue<int>("numLoader");
    ttn->dt_of_load       = bill.memberValue<QDateTime>("dateOfLoad");
    ttn->brutto           = bill.memberValue<int>("bruttoWeight");
    ttn->dt_of_brutto     = bill.memberValue<QDateTime>("dateOfBrutto");
    ttn->driver           = bill.memberValue<int>("driver");//carCodeFromDriver( memberValue<int>("driver", bill) ) ;
    ttn->bum              = bill.memberValue<int>("bum");
    ttn->routed_to_lab    = bill.memberValue<QBitArray>("flags").at(2);
    ttn->num_kart         = byteArrayToString (bill.uid());
    ttn->copy             = 0;
    ttn->time_of_brutto   = ttn->dt_of_brutto.time().toString("hh:mm:ss");
    ttn->brutto_platforma = seq().seqId();


    wrap_async_ex( tr(update_ttn_error_message),
            "Error updating ttn brutto", [&ttn, this]{ asyncFunc().async_update(ttn, t_ttn_name); });
#endif

    //qDebug() << "dateOfLoad: " << bill.memberValue<QDateTime>("dateOfLoad").toString("yyyy-MM-dd hh::mm:ss");

    auto v = ::tools::make_vvector(
                ttn_table.real_field       = bill.memberValue<int>("realNumField"),
                ttn_table.loader           = bill.memberValue<int>("numLoader"),
                ttn_table.dt_of_load       = bill.memberValue<QDateTime>("dateOfLoad").toString(rdb_date_time_format).toAscii().constData(),
                ttn_table.brutto           = bill.memberValue<int>("bruttoWeight"),
                ttn_table.dt_of_brutto     = bill.memberValue<QDateTime>("dateOfBrutto").toString(rdb_date_time_format).toAscii().constData(),
                ttn_table.driver           = bill.memberValue<int>("driver"),
                ttn_table.bum              = bill.memberValue<int>("bum"),
                ttn_table.routed_to_lab    = bill.memberValue<QBitArray>("flags").at(2),
                ttn_table.num_kart         = byteArrayToString (bill.uid()).toAscii().constData(),
                ttn_table.copy             = false,
                ttn_table.time_of_brutto   = bill.memberValue<QDateTime>("dateOfBrutto").time().toString().toAscii().constData(),
                ttn_table.brutto_platforma = seq().seqId()
    );

    current_ttn.set(v);

    async2().exec(
                sql::update(ttn_table).set(v).where(ttn_table.num_nakl == bill.memberValue<int>("billNumber")),
                    tr(update_ttn_error_message)
                );
}

void BeetAcceptanceCulture::updateTaraValues(MifareCardData& bill, boost::mpl::bool_<true> /* pure_weight */)
{
    auto v = ::tools::make_vvector(
                ttn_table.real_bum        = bill.memberValue<int>("bumFact"),
                ttn_table.kagat           = bill.memberValue<int>("kagat"),
                ttn_table.dt_of_unload    = bill.memberValue<QString>("dateOfUnload").toAscii().constData(),
                ttn_table.was_in_lab      = bill.memberValue<QBitArray>("flags").at(3),
                ttn_table.copy            = false,
                ttn_table.tara_platforma  = seq().seqId(),
                ttn_table.field_from_car  = current_car[cars_table.num_field],
                ttn_table.tara            = bill.memberValue<int>("taraWeight"),
                ttn_table.dt_of_tara      = bill.memberValue<QDateTime>("dateOfTara").toString(rdb_date_time_format).toAscii().constData(),
                ttn_table.time_of_tara    = bill.memberValue<QDateTime>("dateOfTara").time().toString().toAscii().constData()
            );

    current_ttn.set(v);

    async2().exec(
                sql::update(ttn_table).set(v).where(
                        ttn_table.num_nakl ==  bill.memberValue<int>("billNumber")),
                    tr(update_ttn_error_message)
                );
}

void BeetAcceptanceCulture::updateTaraValues(MifareCardData& bill, boost::mpl::bool_<false>)
{
    auto v = ::tools::make_vvector (
                    ttn_table.tara            = bill.memberValue<int>("taraWeight"),
                    ttn_table.dt_of_tara      = bill.memberValue<QDateTime>("dateOfTara").toString(rdb_date_time_format).toAscii().constData(),
                    ttn_table.time_of_tara    = bill.memberValue<QDateTime>("dateOfTara").time().toString().toAscii().constData()
                );

    current_ttn.set(v);

    async2().exec(
                sql::update(ttn_table).set(v).
                        where( ttn_table.num_nakl ==  bill.memberValue<int>("billNumber")),
                    tr(update_ttn_error_message)
                );
}
#if 0
void BeetAcceptanceCulture::updateTaraValues(MifareCardData& bill, bool pure_weight)
{

    if ( pure_weight ) {
        ttn->real_bum      = bill.memberValue<int>("bumFact");
        ttn->kagat         = bill.memberValue<int>("kagat");
        ttn->dt_of_unload  = bill.memberValue<QDateTime>("dateOfUnload");
        ttn->was_in_lab    = bill.memberValue<QBitArray>("flags").at(3);


    }

    ttn->copy            = 0;
    ttn->time_of_tara    = ttn->dt_of_tara.time().toString("hh:mm:ss");
    ttn->tara_platforma  = seq().seqId();
    ttn->field_from_car  = car->num_field;



    wrap_async_ex( tr(update_ttn_error_message),
      "Error updating ttn tara: ttn_num: " + QString::number(ttn->num_nakl),
                   [&ttn, this]{ asyncFunc().async_update(ttn, t_ttn_name); });


}
#endif

bool BeetAcceptanceCulture::makeNewTask(MifareCardData& bill)
{
    bill.clear();

    if (current_car[cars_table.num_field] == 0) return false;
#if 0
    QDateTime end_time = timeShitToDateTime( current_car[cars_table.vremja_na_hodku]*60 + dateTimeToTimeShit(QDateTime::currentDateTime())  );

    current_ttn = qx::dao::ptr<t_ttn_beet>(new t_ttn_beet);

    current_ttn->date_time      = QDateTime::currentDateTime();
    current_ttn->car            = current_car->id;
    current_ttn->field          = current_car->num_field;
    current_ttn->driver         = bill.memberValue<uint>("driver");
    current_ttn->time_return    = end_time;
    current_ttn->copy           = 0;
    current_ttn->time_of_return = end_time.toString("hh:mm:ss");
    current_ttn->loader         = current_car->num_loader;



    wrap_async_ex(tr(error_make_new_task), "error make new task",
                  [this]{return asyncFunc().async_insert(current_ttn, false, t_ttn_name);});

    bill.setMemberValue("billNumber", current_ttn->num_nakl);
    bill.setMemberValue("numField"  , current_ttn->field);
#endif
//#warning Need correct insert query here!!!

    QDateTime end_time = timeShitToDateTime( current_car[cars_table.vremja_na_hodku]*60 +
            dateTimeToTimeShit(QDateTime::currentDateTime())  );

    async2().exec(
        sql::insert_into(ttn_table)(ttn_table.date_time,
                                    ttn_table.car,
                                    ttn_table.field,
                                    ttn_table.driver,
                                    ttn_table.time_return,
                                    ttn_table.copy,
                                    ttn_table.time_of_return,
                                    ttn_table.loader)
                .values( QDateTime::currentDateTime().toString(rdb_date_time_format).toAscii().constData(),
                         current_car[cars_table.id],
                         current_car[cars_table.num_field],
                         bill.memberValue<int>("driver"),
                         end_time.toString(rdb_date_time_format).toAscii().constData(),
                         false,
                         end_time.toString("hh:mm:ss").toAscii().constData(),
                         current_car[cars_table.num_loader]
                  ),
        tr(error_make_new_task)
    );

    bill.setMemberValue("billNumber", current_ttn[ttn_table.num_nakl]);
    bill.setMemberValue("numField"  , current_ttn[ttn_table.field]   );

    return true;
}

ReportContext BeetAcceptanceCulture::makeReportContext(qx::dao::ptr<t_cars_beet> car, qx::dao::ptr<t_field_beet> field)
{
#if 0
    qx::dao::ptr<t_kontr_beet> kontr = wrap_async_ex(
          QObject::tr(cant_get_kontr_when_printing),
          "cant get kontr when printing: " + QString::number(kontrCodeFromField( field->id )),
          [&field, this]{return asyncFunc().async_fetch<t_kontr_beet>(
                      kontrCodeFromField( field->id ), t_kontr_name ); });


    qx::dao::ptr<t_const_beet> base_firm         = convienceFunc().getConst<t_const_beet>(seq().appSetting<QString>("base_firm_name"));
    qx::dao::ptr<t_const_beet> dont_check_time   = convienceFunc().getConst<t_const_beet>(seq().appSetting<QString>("dont_check_time_name"));
    qx::dao::ptr<t_const_beet> disp_phone        = convienceFunc().getConst<t_const_beet>(seq().appSetting<QString>("disp_phone_name"));

    QList<ReportsManager::var_instance> vars = QList<ReportsManager::var_instance>{
        ReportsManager::var_instance{"t_ttn", "t_ttn_beet", current_ttn.data()},
        ReportsManager::var_instance{"t_cars", "t_cars_beet", car.data()},
        ReportsManager::var_instance{"t_field", "t_field_beet", field.data()},
        ReportsManager::var_instance{"t_kontr", "t_kontr_beet", kontr.data()},
        ReportsManager::var_instance{"base_firm", "t_const_beet", base_firm.data()},
        ReportsManager::var_instance{"dont_check_time", "t_const_beet", dont_check_time.data()},
        ReportsManager::var_instance{"disp_phone", "t_const_beet", disp_phone.data()}
     };
     return ReportsManager::makeReportContext(vars);
#endif
#warning Insert correct reflection here!!!
    return  ReportContext();
}

ReportContext BeetAcceptanceCulture::finishReport()
{
#if 0
    qx::dao::ptr<t_field_beet> field = wrap_async_ex(tr(cant_get_field_when_printing),
         "finishReport: cant get field " + QString::number(current_ttn->real_field) +
         " when printing for passed ttn: " + QString::number(current_ttn->num_nakl),
         [this]{return asyncFunc().async_fetch<t_field_beet>(
             current_ttn->real_field, t_field_name ); });
    return makeReportContext(current_car, field);
#endif
#warning Insert correct reflection here!!!
    return ReportContext();
}

ReportContext BeetAcceptanceCulture::startReport()
{
#if 0
    qx::dao::ptr<t_field_beet> field = wrap_async_ex(tr(cant_get_field_when_printing),
      "startReport: cant get field " + QString::number(current_ttn->field) +
      " when printing for passed ttn: " + QString::number(current_ttn->num_nakl),
      [this]{return asyncFunc().async_fetch<t_field_beet>(
               current_ttn->field, t_field_name ); });
    return makeReportContext(current_car, field);
#endif
#warning Insert correct reflection here!!!

    return ReportContext();
}

QString BeetAcceptanceCulture::detectPlatformType(const MifareCardData& bill) const
{
    if (!bill.checkMember("bruttoWeight", 0) &&
        !bill.checkMember("dateOfBrutto", timeShitToDateTime(0)))
        return "brutto";

    if ( !bill.checkMember("bumFact", 0) &&
         !bill.checkMember("kagat", 0) &&
         !bill.checkMember("taraWeight", 0) &&
         !bill.checkMember("dateOfTara", timeShitToDateTime(0))) {
        return "brutto"; /*reweighting*/
    }

    if ( bill.checkMember("bumFact", 0) &&
         bill.checkMember("kagat", 0) ) {
        return "tara";
    }

    throw MainSequenceException(tr(autodetect_platform_type_error_message) ,"something terrible happens!!! cant detect platform type. Maybe bill corrupted :(" );

}

bool BeetAcceptanceCulture::isPureBruttoWeight(const MifareCardData& bill) const
{
    if ( bill.memberValue<uint>("bruttoWeight") == 0 ) return true;
    if ( bill.memberValue<uint>("kagat") == 0 ) return false;

    throw MainSequenceException(tr(confuse_brutto_tara_error_message), "confused brutto with tara");
}

bool BeetAcceptanceCulture::isPureTaraWeight(const MifareCardData& bill) const
{
    if ( bill.memberValue<int>("bruttoWeight") > 0 ) return true;
    if ( bill.memberValue<int>("realNumField") == 0 ) return false;

    throw MainSequenceException(tr(forget_brutto_on_tara_error_message), "forget for brutto!" );

}

void BeetAcceptanceCulture::fetchCar(const MifareCardData& bill)
{
/*    current_car = wrap_async_ex(tr(fetch_car_error_message),
           "fetching car failed!!!: driver: " + bill.memberValue<QString>("driver"),
            [&bill, this]{return asyncFunc().async_fetch<t_cars_beet>(
                           carCodeFromDriver( bill.memberValue<uint>("driver") ), t_cars_name  );});


    if (current_car->block) {
        throw MainSequenceException(tr(car_blocked_message), "car is blocked!!!");
    }*/

     current_car = async2().fetch(
                sql::select( cars_table.all ).from(cars_table).where(cars_table.id
                        == (int)carCodeFromDriver( bill.memberValue<uint>("driver"))),
                    tr(fetch_car_error_message)
                  );

     if ( current_car[cars_table.block] ) {
         throw MainSequenceException(tr(car_blocked_message), "car is blocked!!!");
     }
}


} }
