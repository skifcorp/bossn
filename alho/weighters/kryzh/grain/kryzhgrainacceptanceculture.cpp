#include "kryzhgrainacceptanceculture.h"
#include "warnmessages.h"
#include "mainsequence.h"
#include "codeshacks.h"

namespace alho { namespace kryzh {



const QString GrainAcceptanceCulture::t_cars_name("t_cars");
const QString GrainAcceptanceCulture::t_ttn_name("t_ttn");
const QString GrainAcceptanceCulture::t_const_name("t_const");
const QString GrainAcceptanceCulture::t_kontr_name("t_kontr");
const QString GrainAcceptanceCulture::t_bum_name("t_bum");
const QString GrainAcceptanceCulture::t_kagat_name("t_kagat");
const QString GrainAcceptanceCulture::t_field_name("t_field");


QString GrainAcceptanceCulture::bruttoFinishMessage(const MifareCardData& bill )const
{
    return tr(brutto_finish_weight_message).arg( bill.memberValue<QString>("bruttoWeight") );
}

QString GrainAcceptanceCulture::taraFinishMessage(const MifareCardData& )const
{
    return QString::number(current_ttn->tara);
}


void GrainAcceptanceCulture::brutto(int w, MifareCardData& bill ) throw (MainSequenceException)
{
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
               [&bill, this]{return asyncFunc().async_fetch<t_ttn>(
                   bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "GrainAcceptance: brutto weight!, ttn: " << current_ttn->num_nakl;

    repairFieldCodeCorrectnessIfNeeded<t_ttn, t_field>(bill, current_ttn, t_field_name);

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());
    bill.setMemberValue("flags", 2, true );

    //bill.setMemberValue("bum", 99);
    current_ttn->routed_to_lab = 1;


    current_ttn->culture = bill.memberValue<int>("culture");
    current_ttn->repr    = bill.memberValue<int>("repr");
    current_ttn->sort    = bill.memberValue<int>("sort");
    //current_ttn->bum = current_ttn->real_bum = 1;
    //current_ttn->was_in_lab = 1;
    updateBruttoValues(bill, current_ttn);
}

void GrainAcceptanceCulture::tara(int w, MifareCardData& bill) throw (MainSequenceException)
{
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                              [&bill, this]{return asyncFunc().async_fetch<t_ttn>(
                  bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "GrainAcceptance: tara weight!, ttn: " << current_ttn->num_nakl;

    checkTaraByBrutto(w, current_ttn);

    current_ttn->tara = w;
    current_ttn->dt_of_tara = QDateTime::currentDateTime();

    //checkBum(bill);
    //checkLaboratory(current_ttn);

    processDrivingTime(current_ttn, current_car);

    processTaraRupture<t_ttn, t_cars, t_const>(current_ttn, current_car);

    updateTaraValues(bill, current_ttn, current_car, true);

    //chtenie bum i factBum - proveka cho oni estj
    //porverka was_in_lab
    // - kagat
    // - num
    // - repar bum cor
    //ne zabytj pro soobscheie na tablo
}



void GrainAcceptanceCulture::reBrutto(int w, MifareCardData& bill) throw (MainSequenceException)
{
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
        [&bill, this]{return asyncFunc().async_fetch<t_ttn>(
           bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "GrainAcceptance: rebrutto weight!, ttn: " << current_ttn->num_nakl;

    checkBruttoDeltaForReweights(bill.memberValue<uint>("bruttoWeight"), w);

    bill.setMemberValue("flags", 0, true );
    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());
    //bill.setMemberValue("bum", 99);

    updateBruttoValues(bill, current_ttn);
}

void GrainAcceptanceCulture::reTara(int w, MifareCardData& bill) throw (MainSequenceException)
{
    current_ttn = ttnByDriver<t_ttn>( bill.memberValue<uint>("driver") );

    seq().seqDebug() << "GrainAcceptance: retara weight!, ttn: " << current_ttn->num_nakl;

    checkTaraByBrutto(w, current_ttn);

    checkTaraDeltaForReweights(current_ttn->tara, w);

    current_ttn->tara = w;
    current_ttn->dt_of_tara = QDateTime::currentDateTime();

    processDrivingTime(current_ttn, current_car);

    processTaraRupture<t_ttn, t_cars, t_const>(current_ttn, current_car);

    updateTaraValues(bill, current_ttn, current_car, false);
}







void GrainAcceptanceCulture::checkBum( const MifareCardData& bill )const throw(MainSequenceException)
{
    if (bill.memberValue<uint>("bum") != 99 && bill.memberValue<uint>("bumFact") == 0 )
        throw MainSequenceException( tr(car_has_not_been_unloaded),
            "grain car was not unloaded... car " + QString::number(carCodeFromDriver( bill.memberValue<uint>("driver") ) )+
            " num_nakl: " + bill.memberValue<QString>("billNumber") +
            " bum: "      + bill.memberValue<QString>("bum") );
}

void GrainAcceptanceCulture::checkLaboratory( qx::dao::ptr<t_ttn> ttn)const throw(MainSequenceException)
{
    if (ttn->was_in_lab == 0)
        throw MainSequenceException( tr(car_dont_was_in_lab),
                  "grain car was not in lab... car " + QString::number(ttn->car) +
                  " num_nakl: " + QString::number(ttn->num_nakl) ) ;
}




void GrainAcceptanceCulture::updateBruttoValues(MifareCardData& bill, qx::dao::ptr<t_ttn> ttn) throw(MainSequenceException)
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

/*    QStringList lst;
    ttn.isDirty(lst);
    qDebug() << "brutto: isDirty: " << lst;
*/
    wrap_async_ex( tr(update_ttn_error_message), "Error updating ttn brutto",
        [&ttn, this]{ asyncFunc().async_update(ttn, t_ttn_name); });
}

void GrainAcceptanceCulture::updateTaraValues(MifareCardData& bill, qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car, bool pure_weight) throw (MainSequenceException)
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
/*    QStringList lst;
    ttn.isDirty(lst);
    qDebug() << "tara: isDirty: " << lst;
*/
    wrap_async_ex( tr(update_ttn_error_message),
         "Error updating ttn tara: ttn_num: " + QString::number(ttn->num_nakl),
           [&ttn, this]{ asyncFunc().async_update(ttn, t_ttn_name); });
}



bool GrainAcceptanceCulture::makeNewTask(MifareCardData& bill) throw (MainSequenceException)
{
    bill.clear();

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

    //qx::dao::insert(ttn, &database);

    wrap_async_ex(tr(error_make_new_task), "error make new task", [this]{return asyncFunc().async_insert(current_ttn, false, t_ttn_name);});

    bill.setMemberValue("billNumber", current_ttn->num_nakl);
    bill.setMemberValue("numField"  , current_ttn->field);

    return true;
}

ReportContext GrainAcceptanceCulture::makeReportContext(qx::dao::ptr<t_cars> car, qx::dao::ptr<t_field> field)
{

	qx::dao::ptr<t_kontr> kontr = wrap_async_ex(QObject::tr(cant_get_kontr_when_printing), "cant get kontr when printing",
         [&field, this]{return asyncFunc().async_fetch<t_kontr>(
               kontrCodeFromField( field->id  ), t_kontr_name ); });


    qx::dao::ptr<t_const> base_firm         = convienceFunc().getConst<t_const>(seq().appSetting<QString>("base_firm_name"));
    qx::dao::ptr<t_const> dont_check_time   = convienceFunc().getConst<t_const>(seq().appSetting<QString>("dont_check_time_name"));
    qx::dao::ptr<t_const> disp_phone        = convienceFunc().getConst<t_const>(seq().appSetting<QString>("disp_phone_name"));

	QList<ReportsManager::var_instance> vars = QList<ReportsManager::var_instance>{
		ReportsManager::var_instance{"t_ttn", "t_ttn", current_ttn.data()},
		ReportsManager::var_instance{"t_cars", "t_cars", car.data()},
		ReportsManager::var_instance{"t_field", "t_field", field.data()},
		ReportsManager::var_instance{"t_kontr", "t_kontr", kontr.data()},
		ReportsManager::var_instance{"base_firm", "t_const", base_firm.data()},
		ReportsManager::var_instance{"dont_check_time", "t_const", dont_check_time.data()},
		ReportsManager::var_instance{"disp_phone", "t_const", disp_phone.data()}
	 };
	 return ReportsManager::makeReportContext(vars);
}

ReportContext GrainAcceptanceCulture::finishReport() throw(MainSequenceException)
{
	qx::dao::ptr<t_field> field = wrap_async_ex(tr(cant_get_field_when_printing), "finishReport: cant get field " + QString::number(current_ttn->real_field) + " when printing for passed ttn: " + QString::number(current_ttn->num_nakl),
          [this]{return asyncFunc().async_fetch<t_field>(
               current_ttn->real_field, t_field_name ); });
    return makeReportContext(current_car, field);
}

ReportContext GrainAcceptanceCulture::startReport() throw(MainSequenceException)
{
	qx::dao::ptr<t_field> field = wrap_async_ex(tr(cant_get_field_when_printing),
	  "startReport: cant get field " + QString::number(current_ttn->field) +
	  " when printing for passed ttn: " + QString::number(current_ttn->num_nakl),
      [this]{return asyncFunc().async_fetch<t_field>(
              current_ttn->field, t_field_name ); });

    return makeReportContext(current_car, field);

}

QString GrainAcceptanceCulture::detectPlatformType(const MifareCardData& bill) const throw (MainSequenceException)
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

bool GrainAcceptanceCulture::isPureBruttoWeight(const MifareCardData& bill) const throw (MainSequenceException)
{
    if ( bill.memberValue<uint>("bruttoWeight") == 0 ) return true;
    if ( bill.memberValue<uint>("bumFact") == 0 ) return false;

    throw MainSequenceException(tr(confuse_brutto_tara_error_message), "confused brutto with tara");
}

bool GrainAcceptanceCulture::isPureTaraWeight(const MifareCardData& bill) const throw (MainSequenceException)
{
    if ( bill.memberValue<int>("bruttoWeight") > 0 ) return true;
    if ( bill.memberValue<int>("realNumField") == 0 ) return false;

    throw MainSequenceException(tr(forget_brutto_on_tara_error_message), "forget for brutto!" );
}

void GrainAcceptanceCulture::fetchCar(const MifareCardData& bill) throw (MainSequenceException)
{    
    current_car = wrap_async_ex(tr(fetch_car_error_message),
           "fetching car failed!!!: driver: " + bill.memberValue<QString>("driver"),
      [&bill, this]{return asyncFunc().async_fetch<t_cars>(
       carCodeFromDriver( bill.memberValue<uint>("driver") ), t_cars_name );});

    if (current_car->block) {
        throw MainSequenceException(tr(car_blocked_message), "car is blocked!!!");
    }
}

} }
