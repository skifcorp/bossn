#include "kryzhgrainacceptanceculture.h"
#include "warnmessages.h"
#include "mainsequence.h"
#include "codeshacks.h"
#include "reportsmanager2.h"

#include <fusion_tools/make_vvector.h>

#include <boost/fusion/include/vector_tie.hpp>

namespace alho { namespace kryzh {



QString GrainAcceptanceCulture::bruttoFinishMessage(const MifareCardData& bill )const
{
    return tr(brutto_finish_weight_message).arg( bill.memberValue<QString>("bruttoWeight") );
}

QString GrainAcceptanceCulture::taraFinishMessage(const MifareCardData& )const
{
    return  QString::number( current_ttn[ ttn_table.tara ] );//QString::number(current_ttn->tara);
}


void GrainAcceptanceCulture::brutto(int w, MifareCardData& bill )
{
#if 0
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
               [&bill, this]{return asyncFunc().async_fetch<t_ttn>(
                   bill["billNumber"].toUInt(), t_ttn_name);});
#endif
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );


    seq().seqDebug() << "GrainAcceptance: brutto weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    repairFieldCodeCorrectnessIfNeeded2(bill, field_table, ttn_table, current_ttn);

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());
    bill.setMemberValue("flags", 2, true );

    current_ttn[ttn_table.routed_to_lab] = 1;

    current_ttn[ttn_table.culture] = bill.memberValue<int>("culture");
    current_ttn[ttn_table.repr]    = bill.memberValue<int>("repr");
    current_ttn[ttn_table.sort]    = bill.memberValue<int>("sort");

    updateBruttoValues(bill);
}

void GrainAcceptanceCulture::tara(int w, MifareCardData& bill)
{
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );


    seq().seqDebug() << "GrainAcceptance: tara weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    checkTaraByBrutto2(w, ttn_table, current_ttn);

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());


    processDrivingTime2(ttn_table, current_ttn, cars_table, current_car);

    processTaraRupture2(ttn_table, current_ttn, cars_table, current_car);

    updateTaraValues(bill, boost::mpl::true_{});
}



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

void GrainAcceptanceCulture::reTara(int w, MifareCardData& bill)
{
    current_ttn = ttnByDriver2< decltype(current_ttn) >(  bill.memberValue<int>("driver"), ttn_table );

    seq().seqDebug() << "GrainAcceptance: retara weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    checkTaraByBrutto2(w, ttn_table, current_ttn);

    checkTaraDeltaForReweights(current_ttn[ttn_table.tara], w);

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());


    processDrivingTime2(ttn_table, current_ttn, cars_table, current_car);

    processTaraRupture2(ttn_table, current_ttn, cars_table, current_car);

    updateTaraValues(bill, boost::mpl::false_{});
}







void GrainAcceptanceCulture::checkBum( const MifareCardData& bill )const
{
    if (bill.memberValue<uint>("bum") != 99 && bill.memberValue<uint>("bumFact") == 0 )
        throw MainSequenceException( tr(car_has_not_been_unloaded),
            "grain car was not unloaded... car " + QString::number(carCodeFromDriver( bill.memberValue<uint>("driver") ) )+
            " num_nakl: " + bill.memberValue<QString>("billNumber") +
            " bum: "      + bill.memberValue<QString>("bum") );
}

void GrainAcceptanceCulture::checkLaboratory( )const
{
    if (current_ttn[ttn_table.was_in_lab] == false)
        throw MainSequenceException( tr(car_dont_was_in_lab),
            "grain car was not in lab... car " + QString::number(current_ttn[ttn_table.car]) +
                " num_nakl: " + QString::number(current_ttn[ttn_table.num_nakl]) ) ;
}




void GrainAcceptanceCulture::updateBruttoValues(MifareCardData& bill)
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


    wrap_async_ex( tr(update_ttn_error_message), "Error updating ttn brutto",
        [&ttn, this]{ asyncFunc().async_update(ttn, t_ttn_name); });
#endif
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

void GrainAcceptanceCulture::updateTaraValues(MifareCardData& bill, boost::mpl::bool_<true> /* pure_weight */)
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


void GrainAcceptanceCulture::updateTaraValues(MifareCardData& bill, boost::mpl::bool_<false>)
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

bool GrainAcceptanceCulture::makeNewTask(MifareCardData& bill)
{
    bill.clear();

    if (current_car[cars_table.num_field] == 0) return false;
#if 0
    if (current_car->num_field == 0) return false;

    QDateTime end_time = timeShitToDateTime( current_car->vremja_na_hodku*60 + dateTimeToTimeShit(QDateTime::currentDateTime())  );

    current_ttn = qx::dao::ptr<t_ttn>(new t_ttn);

    current_ttn->date_time      = QDateTime::currentDateTime();
    current_ttn->car            = current_car->id;
    current_ttn->field          = current_car->num_field;
    current_ttn->driver         = bill.memberValue<uint>("driver");
    current_ttn->time_return    = end_time;
    current_ttn->copy           = 0;
    current_ttn->time_of_return = end_time.toString("hh:mm:ss");
    current_ttn->loader         = current_car->num_loader;
#endif
    //qx::dao::insert(ttn, &database);
#if 0
    wrap_async_ex(tr(error_make_new_task), "error make new task", [this]{return asyncFunc().async_insert(current_ttn, false, t_ttn_name);});



    bill.setMemberValue("billNumber", current_ttn->num_nakl);
    bill.setMemberValue("numField"  , current_ttn->field);
#endif

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


ReportContext GrainAcceptanceCulture::makeReportContext(int field_id)
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



ReportContext GrainAcceptanceCulture::finishReport()
{
#if 0
    qx::dao::ptr<t_field> field

            = wrap_async_ex(tr(cant_get_field_when_printing), "finishReport: cant get field " + QString::number(current_ttn->real_field) + " when printing for passed ttn: " + QString::number(current_ttn->num_nakl),
          [this]{return asyncFunc().async_fetch<t_field>(
               current_ttn->real_field, t_field_name ); })

            ;
    return makeReportContext(current_car, field);
#endif

    return makeReportContext( current_ttn[ttn_table.real_field] );
}

ReportContext GrainAcceptanceCulture::startReport()
{
#if 0
    qx::dao::ptr<t_field> field

            = wrap_async_ex(tr(cant_get_field_when_printing),
	  "startReport: cant get field " + QString::number(current_ttn->field) +
	  " when printing for passed ttn: " + QString::number(current_ttn->num_nakl),
      [this]{return asyncFunc().async_fetch<t_field>(
              current_ttn->field, t_field_name ); })

            ;

    return makeReportContext(current_car, field);
#endif

    return makeReportContext( current_ttn[ttn_table.field] );
}

QString GrainAcceptanceCulture::detectPlatformType(const MifareCardData& bill) const
{
    if (!bill.checkMember("bruttoWeight", 0) &&
        !bill.checkMember("dateOfBrutto", timeShitToDateTime(0)))
        return "brutto";

    if ( !bill.checkMember("bumFact", 0) &&
         !bill.checkMember("taraWeight", 0) &&
         !bill.checkMember("dateOfTara", timeShitToDateTime(0))) {
        return "brutto"; /*reweighting*/
    }

    if ( bill.checkMember("bumFact", 0) ){
        return "tara";
    }

    throw MainSequenceException(tr(autodetect_platform_type_error_message) ,"something terrible happens!!! cant detect platform type. Maybe bill corrupted :(" );
}

bool GrainAcceptanceCulture::isPureBruttoWeight(const MifareCardData& bill) const
{
    if ( bill.memberValue<uint>("bruttoWeight") == 0 ) return true;
    if ( bill.memberValue<uint>("bumFact") == 0 ) return false;

    throw MainSequenceException(tr(confuse_brutto_tara_error_message), "confused brutto with tara");
}

bool GrainAcceptanceCulture::isPureTaraWeight(const MifareCardData& bill) const
{
    if ( bill.memberValue<int>("bruttoWeight") > 0 ) return true;
    if ( bill.memberValue<int>("realNumField") == 0 ) return false;

    throw MainSequenceException(tr(forget_brutto_on_tara_error_message), "forget for brutto!" );
}

void GrainAcceptanceCulture::fetchCar(const MifareCardData& bill)
{    
#if 0
    current_car = wrap_async_ex(tr(fetch_car_error_message),
           "fetching car failed!!!: driver: " + bill.memberValue<QString>("driver"),
      [&bill, this]{return asyncFunc().async_fetch<t_cars>(
       carCodeFromDriver( bill.memberValue<uint>("driver") ), t_cars_name );});

    if (current_car->block) {
        throw MainSequenceException(tr(car_blocked_message), "car is blocked!!!");
    }
#endif
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
