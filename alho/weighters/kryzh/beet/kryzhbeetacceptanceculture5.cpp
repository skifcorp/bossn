#ifdef AAAA






#include "rdb_pch.h"
#include "kryzhbeetacceptanceculture.h"
#include "mainsequence.h"
#include "codeshacks.h"
#include "kryzhbeettables.h"
#include "reportsmanager2.h"

#include "fusion_tools/make_vvector.h"

#include <boost/fusion/include/vector_tie.hpp>



namespace alho  { namespace kryzh {


void BeetAcceptanceCulture::processFreeBum(MifareCardData & bill)
{
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
}


void BeetAcceptanceCulture::updateBruttoValues(MifareCardData& bill)
{

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
                ttn_table.dt_of_unload    = bill.memberValue<QDateTime>("dateOfUnload").toString(rdb_date_time_format).toAscii().constData(),
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
                    ttn_table.time_of_tara    = bill.memberValue<QDateTime>("dateOfTara").time().toString().toAscii().constData(),
                    ttn_table.copy            = false
                );

    current_ttn.set(v);

    async2().exec(
                sql::update(ttn_table).set(v).
                        where( ttn_table.num_nakl ==  bill.memberValue<int>("billNumber")),
                    tr(update_ttn_error_message)
                );
}


bool BeetAcceptanceCulture::makeNewTask(MifareCardData& bill)
{
    bill.clear();

    if (current_car[cars_table.num_field] == 0) return false;

    QDateTime end_time = timeShitToDateTime( current_car[cars_table.vremja_na_hodku]*60 +
            dateTimeToTimeShit(QDateTime::currentDateTime())  );

    current_ttn[ttn_table.num_nakl] = async2().exec(
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
            ).select_last_insert_id(),
        tr(error_make_new_task)
    ).next().all().front().get<0>();

    bill.setMemberValue("billNumber", current_ttn[ttn_table.num_nakl]);
    bill.setMemberValue("numField"  , current_ttn[ttn_table.field]   );

    return true;
}





uint BeetAcceptanceCulture::countCarsFromFieldForDayExcludeCurrent(int field_num)
{
    try {
        QDate workDate = QTime::currentTime().hour()<8 ? QDate::currentDate().addDays(-1) : QDate::currentDate();

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
    int count = async2().fetch(sql::select(sql::count()).from( t_bum_state_log_table )
            .where( t_bum_state_log_table.date_time >= date_from.toString(rdb_date_time_format).toAscii().constData() &&
                    t_bum_state_log_table.date_time <= date_to.toString(rdb_date_time_format).toAscii().constData() &&
                    t_bum_state_log_table.bum == bum ),
              tr(cant_get_bum_state_log_message) );
    if (count == 0)
        return false;

    auto state = async2().fetch(sql::select(bum_table.state).from(bum_table).where(bum_table.id == bum),
                              tr(cant_get_bum_message));

    return state;
}



void BeetAcceptanceCulture::checkBum( MifareCardData& bill )const
{
    if ( bill.memberValue<int>("bum") == 99 && bill.memberValue<int>("bumFact") == 0 ) {
        bill.setMemberValue("bumFact", 99);
        bill.setMemberValue("kagat", 99);
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

    async2().exec( sql::update(bum_table).set( bum_table.queue = sql::greatest( bum_table.queue - 1, 0 ) ).where( bum_table.id == bum ),
                   tr(clear_bum_queue_error));
}



ReportContext BeetAcceptanceCulture::makeReportContext(int field_id)
{
    auto kontr = async2().fetch( sql::select(kontr_table.all)
                                 .from(kontr_table).where( kontr_table.id == kontrCodeFromField( field_id ) ),
                                 QObject::tr(cant_get_kontr_when_printing) );

    auto field = async2().fetch( sql::select( field_table.all ).from( field_table ).where( field_table.id == field_id ),
                                 tr(cant_get_field_when_printing) );

    auto rc = reports::makeReportContext(
                fusion::vector_tie("t_ttn", current_ttn),
                fusion::vector_tie("t_cars", current_car),
                fusion::vector_tie("t_kontr", kontr),
                fusion::vector_tie("t_field", field) );

    rc["base_firm_value"]       = constantValue<QVariant>(seq().appSetting<QString>("base_firm_name"));
    rc["dont_check_time_value"] = constantValue<QVariant>(seq().appSetting<QString>("dont_check_time_name"));
    rc["disp_phone_value"]      = constantValue<QVariant>(seq().appSetting<QString>("disp_phone_name"));

    return std::move(rc);
}

ReportContext BeetAcceptanceCulture::finishReport()
{
    return makeReportContext( current_ttn[ttn_table.real_field] );
}

ReportContext BeetAcceptanceCulture::startReport()
{
    return makeReportContext( current_ttn[ttn_table.field] );
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

#endif
