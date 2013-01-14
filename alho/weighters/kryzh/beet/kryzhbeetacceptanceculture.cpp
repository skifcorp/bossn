#include "qxorm_pch.h"

#include "kryzhbeetacceptanceculture.h"
#include "mainsequence.h"
#include "codeshacks.h"
#include "kryzhbeettables.h"

namespace alho  { namespace kryzh {

const QString BeetAcceptanceCulture::t_cars_name("t_cars");
const QString BeetAcceptanceCulture::t_ttn_name("t_ttn");
const QString BeetAcceptanceCulture::t_const_name("t_const");
const QString BeetAcceptanceCulture::t_kontr_name("t_kontr");
const QString BeetAcceptanceCulture::t_bum_name("t_bum");
const QString BeetAcceptanceCulture::t_kagat_name("t_kagat");
const QString BeetAcceptanceCulture::t_field_name("t_field");


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
    ret = tr(brutto_finish_weight_message).arg( current_ttn->tara );
    return ret;
}




void BeetAcceptanceCulture::brutto(int w, MifareCardData& bill) throw (MainSequenceException)
{


    /*current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                           [&bill, this]{return asyncFunc().async_fetch<t_ttn_beet>(bill["billNumber"].toUInt(), t_ttn_name);});
*/
    current_ttn_data = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );

    //seq().seqDebug() << "BeetAcceptance: brutto weight!, ttn: " << current_ttn->num_nakl;

    //seq().seqDebug() << "BeetAcceptance: brutto weight!, ttn: " << current_ttn_data.value( ttn_table.num_nakl );

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    repairFieldCodeCorrectnessIfNeeded2(bill, field_table, ttn_table, current_ttn_data);

    processChemicalAnalysis( bill, current_ttn );
    processFreeBum( bill, current_ttn, current_car );

//    if ( bill.memberValue<QBitArray>("flags").at(2) )
//        bill.setMemberValue("flags", 3, true);
//    else
//        bill.setMemberValue("flags", 3, false);

    //current_ttn->sort = bill.memberValue<int>("sort");
    //current_ttn->culture = bill.memberValue<int>("culture");
    //current_ttn->repr = bill.memberValue<int>("repr");

    updateBruttoValues(bill, current_ttn);
    //culture, repr, sort - v bazu
    // - bum
    // rouuted_to_lab = 1
}

void BeetAcceptanceCulture::tara(int w, MifareCardData& bill) throw (MainSequenceException)
{
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                              [&bill, this]{return asyncFunc().async_fetch<t_ttn_beet>(bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "BeetAcceptance: tara weight!, ttn: " << current_ttn->num_nakl;

    checkTaraByBrutto(w, current_ttn);

    current_ttn->tara = w;
    current_ttn->dt_of_tara = QDateTime::currentDateTime();

    checkBum(bill);
    checkLaboratory(bill, current_car);
    checkKagat(bill);

    clearBumQueue(current_ttn);

    processDrivingTime(current_ttn, current_car);

    repairBumCorrectnessIfNeeded(current_ttn);

    processTaraRupture<t_ttn_beet, t_cars_beet, t_const_beet>(current_ttn, current_car);

    updateTaraValues(bill, current_ttn, current_car, true);

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());

}

void BeetAcceptanceCulture::reBrutto(int w, MifareCardData& bill) throw (MainSequenceException)
{
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                           [&bill, this]{return asyncFunc().async_fetch<t_ttn_beet>(bill["billNumber"].toUInt(),t_ttn_name);});

    seq().seqDebug() << "BeetAcceptance: rebrutto weight!, ttn: " << current_ttn->num_nakl;

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

    updateBruttoValues(bill, current_ttn);
}

void BeetAcceptanceCulture::reTara(int w, MifareCardData& bill) throw (MainSequenceException)
{
    qDebug() << "ttn_by_driver";

    current_ttn = ttnByDriver<t_ttn_beet>(  bill.memberValue<uint>("driver") );

    seq().seqDebug() << "BeetAcceptance: retara weight!, ttn: " << current_ttn->num_nakl;

    checkTaraByBrutto(w, current_ttn);

    checkTaraDeltaForReweights(current_ttn->tara, w);

    current_ttn->tara = w;
    current_ttn->dt_of_tara = QDateTime::currentDateTime();

    repairBumCorrectnessIfNeeded(current_ttn);

    processDrivingTime(current_ttn, current_car);

    processTaraRupture<t_ttn_beet, t_cars_beet, t_const_beet>(current_ttn, current_car);

    updateTaraValues(bill, current_ttn, current_car, false);

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());
}


uint BeetAcceptanceCulture::countCarsFromFieldForDayExcludeCurrent(uint ttn_num, uint field_num)  throw()
{
    try {
        QDate workDate = QTime::currentTime().hour()<8 ? QDate::currentDate().addDays(-1) : QDate::currentDate();

        qx_query q("where floor(real_field/100) = "  + QString::number( field_num ) +
                   " and dt_of_brutto>='" + workDate.toString("yyyy.MM.dd") + " 08:00:00'"                            \
                   " and dt_of_brutto<='" + workDate.addDays(1).toString("yyyy.MM.dd") + " 08:00:00' and num_nakl <> " +
                   QString::number(ttn_num) );
        long count = 0;
        asyncFunc().async_count<t_ttn_beet>( count, q, t_ttn_name);

        return count;
    }
    catch (MysqlException& ex) {
        seq().seqWarning() << "cant get count of ttns for chemical analysis!!! field: "<<field_num;
    }

    return 0;
}

bool BeetAcceptanceCulture::analysisEnabled()
{
    qx::dao::ptr<t_const_beet> enab_analysis;
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

    qDebug () << "enable_analysis : " << ret;

    return ret;
}

void BeetAcceptanceCulture::processChemicalAnalysis(MifareCardData & bill, qx::dao::ptr<t_ttn_beet> ttn ) throw()
{
    if ( !analysisEnabled() ) return;

    long count   = countCarsFromFieldForDayExcludeCurrent(ttn->num_nakl,
                                                          kontrCodeFromField( bill.memberValue<uint>("realNumField") ) );

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


bool BeetAcceptanceCulture::checkForNeedDiscreteAnalisys(long count) const throw()
{
    uint num_in_group = (count + 1) % seq().appSetting<uint>("common_size_of_group");
    return num_in_group == seq().appSetting<uint>("common_number_from_group");
}

uint BeetAcceptanceCulture::getAnalisysPeriodFromStorage(uint typ) throw(MysqlException, MainSequenceException)
{
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

    return const_->value.toUInt();
}

bool BeetAcceptanceCulture::checkForNeedDatabaseConstAnalisys(long count, long kontrag)  throw ()
{
    if (count + 1 == 1) return true; //first car always go for analysis

    try {
        auto kontr = wrap_async_ex( "cant get kontr of ttns for chemical analysis!!! kontrag: " + QString::number(kontrag),
                                     QString(),
                [&kontrag, this]{return asyncFunc().async_fetch<t_kontr_beet>(kontrag, t_kontr_name);} );

        uint period = kontr->period;

        if ( period == 0 ) {
            period = wrap_async_ex("error for getting const for type:" + QString::number(kontr->type), QString(),
                [&kontr, this]{return getAnalisysPeriodFromStorage(kontr->type);} );
        }

        uint carInPeriod = kontr->car_in_period;
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
        return true;
    }

}




void BeetAcceptanceCulture::processFreeBum(MifareCardData & bill, qx::dao::ptr<t_ttn_beet> ttn, qx::dao::ptr<t_cars_beet> car) throw(MainSequenceException)
{
    QString bums_where_clause = getBumsClause(bill, car);

    const QString q1 =  "select * from t_bum where state=1 and queue=(select min(queue) from t_bum where state=1 and ("+bums_where_clause+")) and ("+bums_where_clause +");";
    const QString q2 =  "select * from t_bum where queue=(select min(queue) from t_bum where ("+bums_where_clause+")) and ("+bums_where_clause +");";

    qx::dao::ptr<t_bum_beet> bum = wrap_async_ex(tr(get_free_bum_error),
                "Error getting free bum1: ttn: " + QString::number(ttn->num_nakl),
                [&bums_where_clause, &q1, this]{ return asyncFunc().async_exec_query<t_bum_beet>(q1, false);});



    if ( !bum ) {
        bum = wrap_async_ex(tr(get_free_bum_error),
                "Error getting free bum2 ttn: " + QString::number(ttn->num_nakl),
                [&bums_where_clause, &q2, this]{ return asyncFunc().async_exec_query<t_bum_beet>(q2);});
    }


    bum->queue += 1;



    wrap_async_ex( tr(update_bum_queue_error),
                   "Error updating bum queue... ttn: " + QString::number(ttn->num_nakl) +
                   "bum: " + QString::number(bum->id)  +
                   " queue: " + QString::number(bum->queue),
                   [&bum, this]{ asyncFunc().async_update(bum, t_bum_name); });

    if ( bum->id != 99 ) {
        ttn->bum_platforma = bum->id % 10;
        ttn->bum           = bum->id / 10;
                //wrap_async_ex( update_ttn_platform_error, "Error updating ttn platform", [&ttn]{ async_update(ttn); });
        bill.setMemberValue("bum", ttn->bum);
     }
     else {
        ttn->bum           = bum->id;

        bill.setMemberValue("bum", bum->id);
     }
}


QString BeetAcceptanceCulture::getBumsClause(const MifareCardData & bill, qx::dao::ptr<t_cars_beet> car) throw(MainSequenceException)
{
    QStringList ret;
    if (car->dump_body_truck) {
        ret.append("id = 99");
    }

    if (car->side_board) {
        ret.append("(id % 10) = 2");
    }

    if (car->back_board) {
        qx::dao::ptr<t_const_beet> const_ = wrap_async_ex(tr(get_backboard_bum_weight_const_error),
            "error getting const for backboard bum by key: " +
                seq().appSetting<QString>("bum11_name"),
                    [this]{return asyncFunc().async_fetch<t_const_beet>(
                        seq().appSetting<QString>("bum11_name"), t_const_name);});

        if ( bill.memberValue<int>("bruttoWeight") > const_->value.toInt() ) {
            ret.append("id = 11");
        }
        else {
            ret.append("(id % 10) = 1");
        }
    }

    return  ret.join(" or ");
}


void BeetAcceptanceCulture::repairBumCorrectnessIfNeeded(qx::dao::ptr<t_ttn_beet> ttn) throw (MainSequenceException)
{
    if ( ttn->bum == ttn->real_bum ) return;

    int bum = 99;
    if (ttn->bum != 99)
        bum = ttn->bum * 10 + ttn->bum_platforma;

    if ( !checkBumWorks(ttn->dt_of_brutto, ttn->dt_of_tara, bum) ) {
        ttn->bum = ttn->real_bum;
    }
}


bool BeetAcceptanceCulture::checkBumWorks(const QDateTime & date_from, const QDateTime & date_to, long bum) throw(MainSequenceException)
{
    //qx_query q;
    //q.where("date_time").isGreaterThanOrEqualTo(date_from).and_("date_time").isLessThanOrEqualTo(date_to).and_("bum").isEqualTo(QVariant::fromValue<long>(bum));


    const QString q = "select * from t_bum_state_log where date_time >= '" + date_from.toString("yyyy.MM.dd hh:mm:ss") + "' and date_time <= '" + date_to.toString("yyyy.MM.dd hh:mm:ss") + "' and bum = " + QString::number(bum);
    qx::dao::ptr<t_bum_state_log_beet> bum_log = wrap_async_ex( tr(cant_get_bum_state_log_message), "cant get bum state log",
        [&q, this]{ return asyncFunc().async_exec_query<t_bum_state_log_beet>(q, false) ;});



    if (bum_log) return false;

    qx::dao::ptr<t_bum_beet> bum_ptr = wrap_async_ex(tr(cant_get_bum_message), "cant get bum: " + QString::number(bum),
                        [&bum, this]{ return asyncFunc().async_fetch<t_bum_beet>(bum, t_bum_name);});

    return static_cast<bool> (bum_ptr->state);
}



void BeetAcceptanceCulture::checkBum( MifareCardData& bill )const throw(MainSequenceException)
{
    if ( bill.memberValue<int>("bum") == 99 && bill.memberValue<int>("bumFact") == 0 ) {
        bill.setMemberValue("bumFact", 99);
        bill.setMemberValue("kagat", 99);
            //wrap_async_ex( update_bum_queue_error, "Error updating bum queue", [&bum, this]{ async_update(bum); });
    }
}

void BeetAcceptanceCulture::checkLaboratory( const MifareCardData& bill , qx::dao::ptr<t_cars_beet> car)throw(MainSequenceException)
{
    if ( bill.memberValue<QBitArray>("flags").at(2) && !bill.memberValue<QBitArray>("flags").at(3) ) {
        car->block = 1;
        wrap_async_ex( tr(blocking_car_for_lab_error), "Error blocking car which wasnt in lab",
                       [&car, this]{ asyncFunc().async_update(car, t_cars_name); });

        throw MainSequenceException (tr(car_dont_was_in_lab), "car dont was in lab");
    }
}

void BeetAcceptanceCulture::checkKagat(const MifareCardData& bill) throw(MainSequenceException)
{
    if ( bill.memberValue<int>("kagat") ==0 )
        throw MainSequenceException(tr(car_has_not_been_unloaded), "car was not unloaded!");

    qx::dao::ptr<t_kagat_beet> kagat = wrap_async_ex(tr(getting_kagat_error), "cant get kagat: " + bill.memberValue<QString>("kagat"),
       [&bill, this]{ return asyncFunc().async_fetch<t_kagat_beet>(bill.memberValue<int>("kagat"), t_kagat_name) ;});

    if (!kagat->state) {
        throw MainSequenceException(tr(kagat_was_closed_error), "kagat " + bill.memberValue<QString>("kagat") + " was closed!");
    }
}


void BeetAcceptanceCulture::clearBumQueue(qx::dao::ptr<t_ttn_beet> ttn) throw(MainSequenceException)
{
    int bum = ttn->bum;

    if (bum != 99) {
        bum = bum * 10 + ttn->bum_platforma;
    }

    wrap_async_ex(tr(clear_bum_queue_error), "clear bum queue error",
                [&ttn, this, &bum]{ asyncFunc().async_call_query("update t_bum set queue = GREATEST(queue - 1, 0) where id=" + QString::number(bum) + ";") ;});

}

void BeetAcceptanceCulture::updateBruttoValues(MifareCardData& bill, qx::dao::ptr<t_ttn_beet> ttn) throw(MainSequenceException)
{
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
}

void BeetAcceptanceCulture::updateTaraValues(MifareCardData& bill, qx::dao::ptr<t_ttn_beet> ttn, qx::dao::ptr<t_cars_beet> car, bool pure_weight) throw (MainSequenceException)
{
    if ( pure_weight ) {
        ttn->real_bum      = bill.memberValue<int>("bumFact");
        ttn->kagat         = bill.memberValue<int>("kagat");
        ttn->dt_of_unload  = bill.memberValue<QDateTime>("dateOfUnload");
        ttn->was_in_lab    = bill.memberValue<QBitArray>("flags").at(3);

       // seqDebug () << "bum: " << ttn->bum;
    }

    ttn->copy            = 0;
    ttn->time_of_tara    = ttn->dt_of_tara.time().toString("hh:mm:ss");
    ttn->tara_platforma  = seq().seqId();
    ttn->field_from_car  = car->num_field;

    //seqDebug () << "real_rup_tara: " << ttn->real_rup_tara;

    wrap_async_ex( tr(update_ttn_error_message),
      "Error updating ttn tara: ttn_num: " + QString::number(ttn->num_nakl),
                   [&ttn, this]{ asyncFunc().async_update(ttn, t_ttn_name); });
}


bool BeetAcceptanceCulture::makeNewTask(MifareCardData& bill) throw (MainSequenceException)
{
    bill.clear();

    if (current_car->num_field == 0) return false;

    QDateTime end_time = timeShitToDateTime( current_car->vremja_na_hodku*60 + dateTimeToTimeShit(QDateTime::currentDateTime())  );

    current_ttn = qx::dao::ptr<t_ttn_beet>(new t_ttn_beet);

    current_ttn->date_time      = QDateTime::currentDateTime();
    current_ttn->car            = current_car->id;
    current_ttn->field          = current_car->num_field;
    current_ttn->driver         = bill.memberValue<uint>("driver");
    current_ttn->time_return    = end_time;
    current_ttn->copy           = 0;
    current_ttn->time_of_return = end_time.toString("hh:mm:ss");
    current_ttn->loader         = current_car->num_loader;

    //qx::dao::insert(ttn, &database);

    wrap_async_ex(tr(error_make_new_task), "error make new task",
                  [this]{return asyncFunc().async_insert(current_ttn, false, t_ttn_name);});

    bill.setMemberValue("billNumber", current_ttn->num_nakl);
    bill.setMemberValue("numField"  , current_ttn->field);

    return true;
}

ReportContext BeetAcceptanceCulture::makeReportContext(qx::dao::ptr<t_cars_beet> car, qx::dao::ptr<t_field_beet> field)
{

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
}

ReportContext BeetAcceptanceCulture::finishReport() throw(MainSequenceException)
{
    qx::dao::ptr<t_field_beet> field = wrap_async_ex(tr(cant_get_field_when_printing),
         "finishReport: cant get field " + QString::number(current_ttn->real_field) +
         " when printing for passed ttn: " + QString::number(current_ttn->num_nakl),
         [this]{return asyncFunc().async_fetch<t_field_beet>(
             current_ttn->real_field, t_field_name ); });
    return makeReportContext(current_car, field);
}

ReportContext BeetAcceptanceCulture::startReport() throw(MainSequenceException)
{
    qx::dao::ptr<t_field_beet> field = wrap_async_ex(tr(cant_get_field_when_printing),
      "startReport: cant get field " + QString::number(current_ttn->field) +
      " when printing for passed ttn: " + QString::number(current_ttn->num_nakl),
      [this]{return asyncFunc().async_fetch<t_field_beet>(
               current_ttn->field, t_field_name ); });

    return makeReportContext(current_car, field);

}

QString BeetAcceptanceCulture::detectPlatformType(const MifareCardData& bill) const throw (MainSequenceException)
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

bool BeetAcceptanceCulture::isPureBruttoWeight(const MifareCardData& bill) const throw (MainSequenceException)
{
    if ( bill.memberValue<uint>("bruttoWeight") == 0 ) return true;
    if ( bill.memberValue<uint>("kagat") == 0 ) return false;

    throw MainSequenceException(tr(confuse_brutto_tara_error_message), "confused brutto with tara");
}

bool BeetAcceptanceCulture::isPureTaraWeight(const MifareCardData& bill) const throw (MainSequenceException)
{
    if ( bill.memberValue<int>("bruttoWeight") > 0 ) return true;
    if ( bill.memberValue<int>("realNumField") == 0 ) return false;

    throw MainSequenceException(tr(forget_brutto_on_tara_error_message), "forget for brutto!" );

}

void BeetAcceptanceCulture::fetchCar(const MifareCardData& bill) throw (MainSequenceException)
{
/*    current_car = wrap_async_ex(tr(fetch_car_error_message),
           "fetching car failed!!!: driver: " + bill.memberValue<QString>("driver"),
            [&bill, this]{return asyncFunc().async_fetch<t_cars_beet>(
                           carCodeFromDriver( bill.memberValue<uint>("driver") ), t_cars_name  );});


    if (current_car->block) {
        throw MainSequenceException(tr(car_blocked_message), "car is blocked!!!");
    }*/

     current_car_data = async2().fetch(
                sql::select( cars_table.all ).from(cars_table).where(cars_table.id
                        == (int)carCodeFromDriver( bill.memberValue<uint>("driver"))),
                    tr(fetch_car_error_message)
                  );

     if ( current_car_data.value( cars_table.block ) ) {
         throw MainSequenceException(tr(car_blocked_message), "car is blocked!!!");
     }
}


} }
