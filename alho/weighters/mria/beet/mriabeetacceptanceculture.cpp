#include "mriabeetacceptanceculture.h"
#include "mainsequence.h"
#include "codeshacks.h"

namespace alho  { namespace mria {

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
#if 0
    ret = tr(brutto_finish_weight_message).arg( current_ttn->tara );
#endif
    return ret;
}




void BeetAcceptanceCulture::brutto(int w, MifareCardData& bill) throw (MainSequenceException)
{

#if 0
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                           [&bill, this]{return asyncFunc().async_fetch<t_ttn_beet_mria>(bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "BeetAcceptance: brutto weight!, ttn: " << current_ttn->num_nakl;

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    repairFieldCodeCorrectnessIfNeeded<t_ttn_beet_mria, t_field_beet_mria>(bill, current_ttn, t_field_name);

    updateBruttoValues(bill, current_ttn);
#endif

}

void BeetAcceptanceCulture::tara(int w, MifareCardData& bill) throw (MainSequenceException)
{
#if 0
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                              [&bill, this]{return asyncFunc().async_fetch<t_ttn_beet_mria>(bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "BeetAcceptance: tara weight!, ttn: " << current_ttn->num_nakl;

    checkTaraByBrutto(w, current_ttn);

    current_ttn->tara = w;
    current_ttn->dt_of_tara = QDateTime::currentDateTime();

    checkBum(bill);

    clearBumQueue(current_ttn);

    processDrivingTime(current_ttn, current_car);

    

    processTaraRupture<t_ttn_beet_mria, t_cars_beet_mria, t_const_beet_mria>(current_ttn, current_car);

    updateTaraValues(bill, current_ttn, current_car, true);

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());
#endif
}

void BeetAcceptanceCulture::reBrutto(int w, MifareCardData& bill) throw (MainSequenceException)
{
#if 0
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                           [&bill, this]{return asyncFunc().async_fetch<t_ttn_beet_mria>(bill["billNumber"].toUInt(),t_ttn_name);});

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
#endif
}

void BeetAcceptanceCulture::reTara(int w, MifareCardData& bill) throw (MainSequenceException)
{
#if 0
    qDebug() << "ttn_by_driver";

    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                           [&bill, this]{return asyncFunc().async_fetch<t_ttn_beet_mria>(bill["billNumber"].toUInt(),t_ttn_name);});

    seq().seqDebug() << "BeetAcceptance: retara weight!, ttn: " << current_ttn->num_nakl;

    checkTaraByBrutto(w, current_ttn);

    checkTaraDeltaForReweights(current_ttn->tara, w);

    current_ttn->tara = w;
    current_ttn->dt_of_tara = QDateTime::currentDateTime();   

    processDrivingTime(current_ttn, current_car);

    processTaraRupture<t_ttn_beet_mria, t_cars_beet_mria, t_const_beet_mria>(current_ttn, current_car);

    updateTaraValues(bill, current_ttn, current_car, false);

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());
#endif
}

void BeetAcceptanceCulture::checkBum( MifareCardData& bill )const throw(MainSequenceException)
{
    if ( bill.memberValue<int>("bum") == 99 && bill.memberValue<int>("bumFact") == 0 ) {
        bill.setMemberValue("bumFact", 99);
        bill.setMemberValue("kagat", 99);
            //wrap_async_ex( update_bum_queue_error, "Error updating bum queue", [&bum, this]{ async_update(bum); });
    }
}
#if 0
void BeetAcceptanceCulture::clearBumQueue(qx::dao::ptr<t_ttn_beet_mria> ttn) throw(MainSequenceException)
{
    int bum = ttn->bum;

    if (bum != 99) {
        bum = bum * 10 + ttn->bum_platforma;
    }

    wrap_async_ex(tr(clear_bum_queue_error), "clear bum queue error",
                [&ttn, this, &bum]{ asyncFunc().async_call_query("update t_bum set queue = GREATEST(queue - 1, 0) where id=" + QString::number(bum) + ";") ;});

}
#endif
#if 0
void BeetAcceptanceCulture::updateBruttoValues(MifareCardData& bill, qx::dao::ptr<t_ttn_beet_mria> ttn) throw(MainSequenceException)
{
    ttn->real_field       = bill.memberValue<int>("realNumField");
    ttn->loader           = bill.memberValue<int>("numLoader");
    ttn->dt_of_load       = bill.memberValue<QDateTime>("dateOfLoad");
    ttn->brutto           = bill.memberValue<int>("bruttoWeight");
    ttn->dt_of_brutto     = bill.memberValue<QDateTime>("dateOfBrutto");
    ttn->driver           = bill.memberValue<int>("driver");
    ttn->bum              = bill.memberValue<int>("bum");
    ttn->routed_to_lab    = bill.memberValue<QBitArray>("flags").at(2);
    ttn->num_kart         = byteArrayToString (bill.uid());
    ttn->copy             = 0;
    ttn->time_of_brutto   = ttn->dt_of_brutto.time().toString("hh:mm:ss");
    ttn->brutto_platforma = seq().seqId();


    wrap_async_ex( tr(update_ttn_error_message),
            "Error updating ttn brutto", [&ttn, this]{ asyncFunc().async_update(ttn, t_ttn_name); });

}
#endif

#if 0
void BeetAcceptanceCulture::updateTaraValues(MifareCardData& bill, qx::dao::ptr<t_ttn_beet_mria> ttn, qx::dao::ptr<t_cars_beet_mria> car, bool pure_weight) throw (MainSequenceException)
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
#endif


bool BeetAcceptanceCulture::makeNewTask(MifareCardData& bill) throw (MainSequenceException)
{
    bill.clear();
#if 0
    if (current_car->num_field == 0) return false;

    QDateTime end_time = timeShitToDateTime( current_car->vremja_na_hodku*60 + dateTimeToTimeShit(QDateTime::currentDateTime())  );

    current_ttn = qx::dao::ptr<t_ttn_beet_mria>(new t_ttn_beet_mria);

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
#endif
    return true;
}

#if 0
ReportContext BeetAcceptanceCulture::makeReportContext(qx::dao::ptr<t_cars_beet_mria> car, qx::dao::ptr<t_field_beet_mria> field)
{

    qx::dao::ptr<t_kontr_beet_mria> kontr = wrap_async_ex(
          QObject::tr(cant_get_kontr_when_printing),
          "cant get kontr when printing: " + QString::number(kontrCodeFromField( field->id )),
          [&field, this]{return asyncFunc().async_fetch<t_kontr_beet_mria>(
                      kontrCodeFromField( field->id ), t_kontr_name ); });


    qx::dao::ptr<t_const_beet_mria> base_firm         = convienceFunc().getConst<t_const_beet_mria>(seq().appSetting<QString>("base_firm_name"));
    qx::dao::ptr<t_const_beet_mria> dont_check_time   = convienceFunc().getConst<t_const_beet_mria>(seq().appSetting<QString>("dont_check_time_name"));
    qx::dao::ptr<t_const_beet_mria> disp_phone        = convienceFunc().getConst<t_const_beet_mria>(seq().appSetting<QString>("disp_phone_name"));

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

    return ReportContext();
}
#endif
/*
ReportContext BeetAcceptanceCulture::finishReport() throw(MainSequenceException)
{
    qx::dao::ptr<t_field_beet_mria> field = wrap_async_ex(tr(cant_get_field_when_printing),
         "finishReport: cant get field " + QString::number(current_ttn->real_field) +
         " when printing for passed ttn: " + QString::number(current_ttn->num_nakl),
         [this]{return asyncFunc().async_fetch<t_field_beet_mria>(
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
*/
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
    //qDebug() << "database_name:"<< async_.database.databaseName();
#if 0
    current_car = wrap_async_ex(tr(fetch_car_error_message),
           "fetching car failed!!!: driver: " + bill.memberValue<QString>("driver"),
            [&bill, this]{return asyncFunc().async_fetch<t_cars_beet_mria>(
                           bill.memberValue<uint>("driver"), t_cars_name  );});

    if (current_car->block) {
        throw MainSequenceException(tr(car_blocked_message), "car is blocked!!!");
    }
#endif
}

void BeetAcceptanceCulture::checkPerimetr() throw (MainSequenceException)
{
#if 0
    if (current_car->fl_perimetr==false)
        seq().processPerimeter();
#endif
}




} }
