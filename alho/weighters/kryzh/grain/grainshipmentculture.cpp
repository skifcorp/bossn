#include "grainshipmentculture.h"
#include "graindbstructs.h"
#include "codeshacks.h"

namespace alho { namespace kryzh {


const QString GrainShipmentCulture::t_cars_name("t_cars");
const QString GrainShipmentCulture::t_ttn_name("t_ttno");
const QString GrainShipmentCulture::t_const_name("t_const");
const QString GrainShipmentCulture::t_kontr_name("t_kontr");
//const QString GrainShipmentCulture::t_bum_name("t_bum");
//const QString GrainShipmentCulture::t_kagat_name("t_kagat");
//const QString GrainShipmentCulture::t_field_name("t_field");
const QString GrainShipmentCulture::t_prikaz_name("t_prikaz");



QString GrainShipmentCulture::bruttoFinishMessage(const MifareCardData&  )const
{
    return tr(brutto_finish_weight_message).arg( current_ttn->brutto );
}

QString GrainShipmentCulture::taraFinishMessage(const MifareCardData& bill)const
{
    return tr("tara:") + bill.memberValue<QString>("taraWeight") + tr("rest:")
            + QString::number(current_prikaz->lim - current_prikaz->rest -
                              current_prikaz->virtual_rest);
}




void GrainShipmentCulture::updateBruttoValues(MifareCardData& , qx::dao::ptr<t_ttno> ttn) throw(MainSequenceException)
{
    wrap_async_ex( tr(update_ttn_error_message), "Error updating ttn brutto",
        [&ttn, this]{ asyncFunc().async_update(ttn, t_ttn_name); });
}

void GrainShipmentCulture::updateTaraValues(MifareCardData& bill, qx::dao::ptr<t_ttno> ttn) throw (MainSequenceException)
{    
    ttn->num_kart         = byteArrayToString (bill.uid());
    //ttn->driver           = bill.memberValue<int>("driver");
    ttn->copy             = 0;    

    wrap_async_ex( tr(update_ttn_error_message),
         "Error updating ttn tara: ttn_num: " + QString::number(ttn->num_nakl),
           [&ttn, this]{ asyncFunc().async_update(ttn, t_ttn_name); });
}




ReportContext GrainShipmentCulture::makeReportContext( qx::dao::ptr<t_ttno> ttn )
{
    qx::dao::ptr<t_kontr> kontr = wrap_async_ex(QObject::tr(cant_get_kontr_when_printing), "cant get kontr when printing",
         [this]{return asyncFunc().async_fetch<t_kontr>(
               kontrCodeFromField( current_prikaz->place_in ), t_kontr_name ); });


    qx::dao::ptr<t_const> base_firm         = convienceFunc().getConst<t_const>(seq().appSetting<QString>("base_firm_name"));
    qx::dao::ptr<t_const> disp_phone        = convienceFunc().getConst<t_const>(seq().appSetting<QString>("disp_phone_name"));

    QList<ReportsManager::var_instance> vars = QList<ReportsManager::var_instance>{
        ReportsManager::var_instance{"t_ttn", "t_ttno", ttn.data()},
        ReportsManager::var_instance{"t_cars", "t_cars", current_car.data()},
        ReportsManager::var_instance{"t_kontr", "t_kontr", kontr.data()},
        ReportsManager::var_instance{"base_firm", "t_const", base_firm.data()},
        ReportsManager::var_instance{"disp_phone", "t_const", disp_phone.data()},
        ReportsManager::var_instance{"t_prikaz", "t_prikaz", current_prikaz.data()}
     };
     return ReportsManager::makeReportContext(vars);
}

ReportContext GrainShipmentCulture::finishReport() throw(MainSequenceException)
{
    return makeReportContext(current_ttn);
}


ReportContext GrainShipmentCulture::startReport() throw(MainSequenceException)
{
    return makeReportContext(current_ttn);
}


QString GrainShipmentCulture::detectPlatformType(const MifareCardData& bill) const throw (MainSequenceException)
{
    if (!bill.checkMember("taraWeight", 0) &&
        !bill.checkMember("dateOfTara", timeShitToDateTime(0)))
        return "tara";


    return "brutto";
}

bool GrainShipmentCulture::isPureBruttoWeight(const MifareCardData& bill) const throw (MainSequenceException)
{
//    if (  bill.memberValue<uint>("taraWeight") == 0 )
//        throw MainSequenceException(tr(forget_tara_on_brutto_error_message), "forget for tara!" );

    if ( bill.memberValue<uint>("bruttoWeight") == 0 &&
         bill.memberValue<uint>("taraWeight") != 0) return true;


    return false;
}

bool GrainShipmentCulture::isPureTaraWeight(const MifareCardData& bill) const throw (MainSequenceException)
{
    if ( bill.memberValue<uint>("taraWeight") == 0 ) return true;

    if ( bill.memberValue<uint>("bruttoWeight") == 0 ) return false;

    throw MainSequenceException(tr(confuse_brutto_tara_error_message ), "confused brutto with tara"  );
}

void GrainShipmentCulture::fetchCar(const MifareCardData& bill) throw (MainSequenceException)
{
    //qDebug() << "database_name:"<< async_.database.databaseName();

    current_car = wrap_async_ex(tr(fetch_car_error_message),
           "fetching car failed!!!: driver: " + bill.memberValue<QString>("driver"),
      [&bill, this]{return asyncFunc().async_fetch<t_cars>(
       carCodeFromDriver( bill.memberValue<uint>("driver") ), t_cars_name );});

    if (current_car->block) {
        throw MainSequenceException(tr(car_blocked_message), "car is blocked!!!");
    }
}


void GrainShipmentCulture::brutto(int w, MifareCardData& bill) throw (MainSequenceException)
{
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
               [&bill, this]{return asyncFunc().async_fetch<t_ttno>(
                   bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "GrainShipment: brutto weight!, ttn: " << current_ttn->num_nakl;

    current_ttn->brutto           = w;
    current_ttn->dt_of_brutto     = QDateTime::currentDateTime();

    checkBruttoByTara(w, current_ttn);

    current_prikaz = fetchPrikaz(current_ttn->prikaz);

    checkPrikazClosed(current_prikaz);
    checkPrikazLimit( current_prikaz );


    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    current_prikaz->rest         += current_ttn->netto();
    //qx::dao::ptr<t_ttno> ttn_max_netto = ttnWithMaxNetto();

    //qDebug() << "virtual rest: 1 " << current_prikaz->virtual_rest;

    //if (ttn_max_netto) {
    //    qDebug()  << "ttn_max_netto: " <<
    //                 ttn_max_netto->num_nakl << " " << ttn_max_netto->netto();
//
  //      current_prikaz->virtual_rest -= ttn_max_netto->netto();
    //}


    //current_prikaz->virtual_rest += current_ttn->netto();

    qDebug() << "virtual rest: 2 " << current_prikaz->virtual_rest;

    updateBruttoValues(bill, current_ttn);
    updatePrikaz(current_prikaz);
}

void GrainShipmentCulture::checkPrikazLimit(qx::dao::ptr<t_prikaz> pr) throw (MainSequenceException)
{
    if ( pr->rest + current_ttn->netto() > pr->lim )
        throw MainSequenceException(tr(car_has_netto_overlimit) + " " +
           QString::number(pr->rest + current_ttn->netto() - pr->lim),
           "netto overlimit: ttn: " + QString::number(current_ttn->num_nakl) +
           " prikaz: " + QString::number(pr->num_nakl) +
           " delta: " + QString::number(pr->rest + current_ttn->netto() - pr->lim) );
}

void GrainShipmentCulture::reBrutto(int w, MifareCardData& bill) throw (MainSequenceException)
{
    current_ttn = ttnByDriver<t_ttno>( bill.memberValue<uint>("driver") );

    seq().seqDebug() << "GrainShipment: retara weight!, ttn: " << current_ttn->num_nakl;



    checkBruttoByTara(w, current_ttn);

    checkBruttoDeltaForReweights(current_ttn->brutto, w);

    current_prikaz =  fetchPrikaz(current_ttn->prikaz);
    checkPrikazClosed( current_prikaz );
    checkPrikazLimit( current_prikaz );

    rollbackPrikaz( current_prikaz, current_ttn );

    current_ttn->brutto = w;
    current_ttn->dt_of_brutto = QDateTime::currentDateTime();

    current_prikaz->rest += current_ttn->netto();

    //qx::dao::ptr<t_ttno> ttn_max_netto = ttnWithMaxNetto();
    //if (ttn_max_netto)
    //   current_prikaz->virtual_rest -= ttn_max_netto->netto();

    //current_prikaz->virtual_rest += current_ttn->netto();


    updateBruttoValues(bill, current_ttn);
    updatePrikaz(current_prikaz);
}


void GrainShipmentCulture::rollbackPrikaz(qx::dao::ptr<t_prikaz> pr,
                                           qx::dao::ptr<t_ttno> ttn)
{
    pr->rest -= ttn->netto();
}

void GrainShipmentCulture::tara(int w, MifareCardData& bill) throw (MainSequenceException)
{
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                              [&bill, this]{return asyncFunc().async_fetch<t_ttno>(
                  bill["billNumber"].toUInt(), t_ttn_name);});

    seq().seqDebug() << "GrainShipment: tara weight!, ttn: " << current_ttn->num_nakl;

    current_prikaz = fetchPrikaz(current_ttn->prikaz);

    checkPrikazClosed( current_prikaz );

    //current_prikaz->virtual_rest += getMaxNetto(current_prikaz);

    checkVirtualNetto(current_prikaz);

    current_ttn->tara = w;
    current_ttn->dt_of_tara = QDateTime::currentDateTime();

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());

    //processTaraRupture<t_ttno, t_cars, t_const>(current_ttn, current_car);

    updateTaraValues(bill, current_ttn);
    updatePrikaz(current_prikaz);
}

void GrainShipmentCulture::reTara(int w, MifareCardData& bill) throw (MainSequenceException)
{
    current_ttn = wrap_async_ex(tr(fetch_ttn_error_message), "fetching ttn failed!!!",
                              [&bill, this]{return asyncFunc().async_fetch<t_ttno>(
                  bill["billNumber"].toUInt(), t_ttn_name);});


    seq().seqDebug() << "GrainShipment: retara weight!, ttn: " << current_ttn->num_nakl;

    current_prikaz = fetchPrikaz(current_ttn->prikaz);

    checkPrikazClosed( current_prikaz  );

    checkTaraDeltaForReweights(current_ttn->tara, w);


    //getMaxNetto(current_prikaz);

    checkVirtualNetto(current_prikaz);

    current_ttn->tara = w;
    current_ttn->dt_of_tara = QDateTime::currentDateTime();

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());

    //processTaraRupture<t_ttno, t_cars, t_const>(current_ttn, current_car);

    updateTaraValues(bill, current_ttn);
}

qx::dao::ptr<t_prikaz>  GrainShipmentCulture::fetchPrikaz(int num) throw (MainSequenceException)
{
    //const QString q = "select * from t_prikaz where num_nakl = " + QString::number(num);
    auto p = wrap_async_ex(tr(cant_fetch_prikaz), "cant fetch prikaz for " + QString::number(num),
          [this, &num]{return asyncFunc().async_fetch<t_prikaz>(num, t_prikaz_name);});

    return p;

}

void GrainShipmentCulture::checkPrikazClosed( qx::dao::ptr<t_prikaz> pr ) throw (MainSequenceException)
{
    if ( pr->rest >= pr->lim )
        throw MainSequenceException(tr(prikaz_already_closed),
            "prikaz already closed: " + QString::number(pr->num_nakl));
}

bool GrainShipmentCulture::makeNewTask(MifareCardData &bill) throw (MainSequenceException)
{
    bill.clear();


    current_ttn = qx::dao::ptr<t_ttno>(new t_ttno);

    current_ttn->date_time      = QDateTime::currentDateTime();
    current_ttn->car            = current_car->id;
    current_ttn->driver         = bill.memberValue<uint>("driver");
    current_ttn->copy           = 0;
    current_ttn->prikaz         = current_prikaz->num_nakl;


    wrap_async_ex(tr(error_make_new_task), "error make new task",
                    [this]{return asyncFunc().async_insert(current_ttn, false, t_ttn_name);});

    bill.setMemberValue("billNumber", current_ttn->num_nakl);

    return true;
}


void GrainShipmentCulture::updatePrikaz( qx::dao::ptr<t_prikaz> pr) throw (MainSequenceException)
{
    wrap_async_ex( tr(cant_update_prikaz),  "cant update prikaz",
                   [this, &pr]{ return asyncFunc().async_update(pr, t_prikaz_name); });
}



qx::dao::ptr<t_ttno> GrainShipmentCulture::ttnWithMaxNetto() throw (MainSequenceException)
{
    const QString q = "select * from t_ttno where brutto - tara = " \
       "(select MAX(brutto-tara) from t_ttno where car=" + QString::number(current_car->id) +
       " AND num_nakl != " + QString::number(current_ttn->num_nakl) + " and brutto > 0  LIMIT 0,1)";

    qx::dao::ptr<t_ttno> ttn_max_netto = wrap_async_ex( tr(cant_get_ttn_with_max_netto),
       "cant get ttn with max netto for car: " + QString::number(current_car->id),
        [this, &q]{return asyncFunc().async_exec_query<t_ttno>(q, false); }  );

    return ttn_max_netto;

}


#if 0

select brutto-tara from t_ttno where num_nakl in (
  select num_nakl from t_ttno where brutto - tara =
    (select MAX(brutto - tara) from t_ttno where car in
      (select car from t_cars inner join t_ttno on t_cars.id = t_ttno.car where
        t_ttno.tara != 0 and t_ttno.brutto = 0 and t_ttno.prikaz = 1 ) AND brutto > 0))


#endif


#if 0


select sum(netto) from (
 select MAX(t_ttn1.brutto - t_ttn1.tara) AS netto from t_ttn AS t_ttn1
   inner join t_ttn AS t_ttn2 on t_ttn1.car = t_ttn2.car
     where t_ttn2.brutto != 0 and t_ttn2.tara = 0
       group by t_ttn1.car
) as netto_sum;


#endif

int GrainShipmentCulture::getMaxNetto(qx::dao::ptr<t_prikaz> pr) throw(MainSequenceException)
{
    qx::dao::ptr<t_ttno> ttn_max_netto = ttnWithMaxNetto();
    int ret = 0;
    if (!ttn_max_netto) {
        qWarning() << "ttn_with_max_netto == nullptr ... car: " <<current_car->id;
    }
    else {
        if ( pr->virtual_rest + ttn_max_netto->netto() > pr->lim ) {
            throw MainSequenceException(tr(car_netto_too_big_for_prikaz),
                "car " + QString::number(current_car->id) +
                " has max netto: " + QString::number(ttn_max_netto->num_nakl) +
                " by ttn: " + QString::number(ttn_max_netto->num_nakl) +
                " prikaz: " + QString::number(pr->num_nakl) +
                " virtual rest: " + QString::number(pr->virtual_rest) +
                " limit: " + QString::number(pr->rest));
        }

        ret = ttn_max_netto->netto();
    }

    return ret;
}

int GrainShipmentCulture::getVirtualNetto(qx::dao::ptr<t_prikaz> pr)
  throw (MainSequenceException)
{
    const QString q =
            "select sum(netto) from (" \
                "select MAX(t_ttn1.brutto - t_ttn1.tara) AS netto from t_ttno AS t_ttn1 " \
                  "inner join t_ttno AS t_ttn2 on t_ttn1.car = t_ttn2.car " \
                      " where ((t_ttn2.tara != 0 and t_ttn2.brutto = 0) or " \
                      " (t_ttn2.car = " + QString::number(current_car->id) + ")) and "    \
                      " t_ttn1.brutto > t_ttn1.tara and t_ttn1.prikaz = " +
                      QString::number(pr->num_nakl) +
                      " group by t_ttn1.car  "\
            ") as netto_sum;";

    int virtual_netto = wrap_async_ex( tr(cant_get_virtual_netto),
               "cant get virtual netto for prikaz + " + QString::number(pr->num_nakl),
            [this, &q]{return asyncFunc().async_call_query<int>(q);});

    return virtual_netto;
}

void GrainShipmentCulture::checkVirtualNetto(qx::dao::ptr<t_prikaz> pr) throw (MainSequenceException)
{
    int vn = getVirtualNetto(pr);

    pr->virtual_rest = vn;

    if ( pr->rest + vn > pr->lim )
        throw MainSequenceException(tr(car_netto_too_big_for_prikaz),
            "car " + QString::number(current_car->id) +
            //" has max netto: " + QString::number(ttn_max_netto->num_nakl) +
            " by ttn: " + QString::number(current_ttn->num_nakl) +
            " prikaz: " + QString::number(pr->num_nakl) +
            " virtual rest: " + QString::number(vn) +
            " limit: " + QString::number(pr->rest));

}

} }
