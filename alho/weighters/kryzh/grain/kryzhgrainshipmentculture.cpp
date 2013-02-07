#include "kryzhgrainshipmentculture.h"
#include "kryzhgraindbstructs.h"
#include "codeshacks.h"
#include "reportsmanager2.h"

#include <fusion_tools/make_vvector.h>

#include <boost/fusion/include/vector_tie.hpp>

#include <mpllibs/metaparse/string.hpp>

#define _S(X) MPLLIBS_STRING(X)

namespace alho { namespace kryzh {

QString GrainShipmentCulture::bruttoFinishMessage(const MifareCardData&  )const
{
    return tr(brutto_finish_weight_message).arg( current_ttn[ttn_table.brutto] );
}

QString GrainShipmentCulture::taraFinishMessage(const MifareCardData& bill)const
{
    return tr("tara:") + bill.memberValue<QString>("taraWeight") + tr("rest:")
            + QString::number(current_prikaz[prikaz_table.lim] - current_prikaz[prikaz_table.rest] -
                              current_prikaz[prikaz_table.virtual_rest]);
}

void GrainShipmentCulture::updateBruttoValues(MifareCardData& bill)
{
#if 0
    wrap_async_ex( tr(update_ttn_error_message), "Error updating ttn brutto",
        [&ttn, this]{ asyncFunc().async_update(ttn, t_ttn_name); });
#endif
    auto v = ::tools::make_vvector(
                ttn_table.brutto           = bill.memberValue<int>("bruttoWeight"),
                ttn_table.dt_of_brutto     = bill.memberValue<QDateTime>("dateOfBrutto").toString(rdb_date_time_format).toAscii().constData()
            );

    current_ttn.set(v);

    async2().exec(
                sql::update(ttn_table).set(v).where(ttn_table.num_nakl == bill.memberValue<int>("billNumber")),
                    tr(update_ttn_error_message)
                );

}



void GrainShipmentCulture::updateTaraValues(MifareCardData& bill)
{    
#if 0
    ttn->num_kart         = byteArrayToString (bill.uid());
    //ttn->driver           = bill.memberValue<int>("driver");
    ttn->copy             = 0;    

    wrap_async_ex( tr(update_ttn_error_message),
         "Error updating ttn tara: ttn_num: " + QString::number(ttn->num_nakl),
           [&ttn, this]{ asyncFunc().async_update(ttn, t_ttn_name); });
#endif


    auto v = ::tools::make_vvector(
                ttn_table.tara           = bill.memberValue<int>("taraWeight"),
                ttn_table.dt_of_tara     = bill.memberValue<QDateTime>("dateOfTara").toString(rdb_date_time_format).toAscii().constData(),
                ttn_table.num_kart       = byteArrayToString (bill.uid()).toAscii().constData(),
                ttn_table.copy           = false
            );
    current_ttn.set(v);

    async2().exec(
                sql::update(ttn_table).set(v).where(ttn_table.num_nakl == bill.memberValue<int>("billNumber")),
                    tr(update_ttn_error_message)
                );


}




ReportContext GrainShipmentCulture::makeReportContext(  )
{
#if 0
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

    return ReportContext();
#endif

    auto kontr = async2().fetch( sql::select(kontr_table.all)
                    .from(kontr_table).where( kontr_table.id == current_prikaz[prikaz_table.place_in] ),
                          QObject::tr(cant_get_kontr_when_printing) );


    auto rc = reports::makeReportContext(
            fusion::vector_tie("t_ttn", current_ttn),
            fusion::vector_tie("t_cars", current_car),
            fusion::vector_tie("t_kontr", kontr),
            fusion::vector_tie("t_prikaz", current_prikaz) );

    rc["base_firm_value"]       = constantValue<QVariant>(seq().appSetting<QString>("base_firm_name"));
    rc["dont_check_time_value"] = constantValue<QVariant>(seq().appSetting<QString>("dont_check_time_name"));
    rc["disp_phone_value"]      = constantValue<QVariant>(seq().appSetting<QString>("disp_phone_name"));

    return std::move(rc);
}


ReportContext GrainShipmentCulture::finishReport()
{
    return makeReportContext();
}


ReportContext GrainShipmentCulture::startReport()
{
    return makeReportContext();
}


QString GrainShipmentCulture::detectPlatformType(const MifareCardData& bill) const
{
    if (!bill.checkMember("taraWeight", 0) &&
        !bill.checkMember("dateOfTara", timeShitToDateTime(0)))
        return "tara";


    return "brutto";
}

bool GrainShipmentCulture::isPureBruttoWeight(const MifareCardData& bill) const
{
    if ( bill.memberValue<uint>("bruttoWeight") == 0 &&
         bill.memberValue<uint>("taraWeight") != 0) return true;

    return false;
}

bool GrainShipmentCulture::isPureTaraWeight(const MifareCardData& bill) const
{
    if ( bill.memberValue<uint>("taraWeight") == 0 ) return true;

    if ( bill.memberValue<uint>("bruttoWeight") == 0 ) return false;

    throw MainSequenceException(tr(confuse_brutto_tara_error_message ), "confused brutto with tara"  );
}

void GrainShipmentCulture::fetchCar(const MifareCardData& bill)
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


void GrainShipmentCulture::brutto(int w, MifareCardData& bill)
{
#if 0
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

    qDebug() << "virtual rest: 2 " << current_prikaz->virtual_rest;

    updateBruttoValues(bill, current_ttn);
    updatePrikaz(current_prikaz);
#endif
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );

    seq().seqDebug() << "GrainShipment: brutto weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    checkBruttoByTara2(w, ttn_table, current_ttn);

    fetchPrikaz(current_ttn[ttn_table.prikaz]);

    checkPrikazClosed();

    checkPrikazLimit();

    current_prikaz[ prikaz_table.rest ] += ((current_ttn[ttn_table.brutto] - current_ttn[ttn_table.tara]));

    updateBruttoValues(bill);

    updateRest();
}


void GrainShipmentCulture::checkPrikazLimit()
{
    if ( current_prikaz[prikaz_table.rest] + current_ttn[ttn_table.brutto] -
         current_ttn[ttn_table.tara] > current_prikaz[prikaz_table.lim] )
        throw MainSequenceException( tr(car_has_netto_overlimit) + " " +
           QString::number( current_prikaz[prikaz_table.rest] + current_ttn[ttn_table.brutto] -
            current_ttn[ttn_table.tara] - current_prikaz[prikaz_table.lim] ),
           "netto overlimit: ttn: " + QString::number(current_ttn[ttn_table.num_nakl]) +
            " prikaz: " + QString::number(current_prikaz[ prikaz_table.num_nakl] ) +
            " delta: " + QString::number( current_prikaz[prikaz_table.rest] +
            current_ttn[ttn_table.brutto] - current_ttn[ttn_table.tara] - current_prikaz[prikaz_table.lim]) );
}



void GrainShipmentCulture::reBrutto(int w, MifareCardData& bill)
{
#if 0
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
#endif
    current_ttn = ttnByDriver2< decltype(current_ttn) >(  bill.memberValue<int>("driver"), ttn_table );

    seq().seqDebug() << "GrainShipment: retara weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    bill.setMemberValue("bruttoWeight", w);
    bill.setMemberValue("dateOfBrutto", QDateTime::currentDateTime());

    checkBruttoByTara2(w, ttn_table, current_ttn);

    checkBruttoDeltaForReweights(current_ttn[ttn_table.brutto], w);

    fetchPrikaz(current_ttn[ttn_table.prikaz]);

    checkPrikazClosed();

    checkPrikazLimit();

    rollbackPrikaz( );

    current_prikaz[ prikaz_table.rest ] += ((current_ttn[ttn_table.brutto] - current_ttn[ttn_table.tara]));

    updateBruttoValues(bill);

    updateRest();
}


void GrainShipmentCulture::rollbackPrikaz()
{
    current_prikaz[ prikaz_table.rest ] -= ( current_ttn [ttn_table.brutto] - current_ttn[ttn_table.tara] );
}



void GrainShipmentCulture::tara(int w, MifareCardData& bill)
{
#if 0
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
    //updatePrikaz(current_prikaz);
#endif
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );

    seq().seqDebug() << "GrainShipment: tara weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    fetchPrikaz(current_ttn[ttn_table.prikaz]);

    checkPrikazClosed( );

    checkVirtualNetto( );

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());

    updateTaraValues(bill);
    updateVirtualRest();
}

void GrainShipmentCulture::reTara(int w, MifareCardData& bill)
{
#if 0
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
#endif
    current_ttn = async2().fetch(
                sql::select( ttn_table.all ).from(ttn_table).where( ttn_table.num_nakl ==  bill["billNumber"].toInt()),
                tr(fetch_ttn_error_message) );

    seq().seqDebug() << "GrainShipment: retara weight!, ttn: " << current_ttn[ttn_table.num_nakl];

    checkTaraDeltaForReweights(current_ttn[ttn_table.tara], w);

    fetchPrikaz(current_ttn[ttn_table.prikaz]);

    checkPrikazClosed( );

    checkVirtualNetto();

    bill.setMemberValue("taraWeight", w);
    bill.setMemberValue("dateOfTara", QDateTime::currentDateTime());

    updateTaraValues(bill);
    updateVirtualRest();
}


void GrainShipmentCulture::fetchPrikaz(int num)
{
    current_prikaz = async2().fetch(sql::select(prikaz_table.all).from(prikaz_table).where( prikaz_table.num_nakl == num ),
                                    tr(cant_fetch_prikaz) );
}



void GrainShipmentCulture::checkPrikazClosed( )  const
{
    if ( current_prikaz[prikaz_table.rest] >= current_prikaz[prikaz_table.lim] )
        throw MainSequenceException(tr(prikaz_already_closed),
            "prikaz already closed: " + QString::number( current_prikaz[prikaz_table.num_nakl]));
}



bool GrainShipmentCulture::makeNewTask(MifareCardData &bill)
{
    bill.clear();
#if 0

    current_ttn = qx::dao::ptr<t_ttno>(new t_ttno);

    current_ttn->date_time      = QDateTime::currentDateTime();
    current_ttn->car            = current_car->id;
    current_ttn->driver         = bill.memberValue<uint>("driver");
    current_ttn->copy           = 0;
    current_ttn->prikaz         = current_prikaz->num_nakl;


    wrap_async_ex(tr(error_make_new_task), "error make new task",
                    [this]{return asyncFunc().async_insert(current_ttn, false, t_ttn_name);});

    bill.setMemberValue("billNumber", current_ttn->num_nakl);
#endif
    async2().exec(
        sql::insert_into(ttn_table)(ttn_table.date_time,
                                       ttn_table.car,
                                       ttn_table.driver,
                                       ttn_table.copy,
                                       ttn_table.prikaz)
                .values(
                        QDateTime::currentDateTime().toString(rdb_date_time_format).toAscii().constData(),
                        current_car[cars_table.id],
                        bill.memberValue<int>("driver"),
                        false,
                        current_prikaz[prikaz_table.num_nakl]
                    ),
                    tr(error_make_new_task)
                );

    bill.setMemberValue("billNumber", current_ttn[ttn_table.num_nakl]);

    return true;
}

void GrainShipmentCulture::updateVirtualRest()
{
    async2().exec( sql::update( prikaz_table )
                   .set( prikaz_table.virtual_rest = current_prikaz[prikaz_table.virtual_rest] ),
                   tr(cant_update_prikaz) );
}


void GrainShipmentCulture::updateRest()
{
    async2().exec( sql::update( prikaz_table )
                   .set( prikaz_table.rest = current_prikaz[prikaz_table.rest] ),
                   tr(cant_update_prikaz) );
}

#if 0
void GrainShipmentCulture::updatePrikaz( qx::dao::ptr<t_prikaz> pr) throw (MainSequenceException)
{

    wrap_async_ex( tr(cant_update_prikaz),  "cant update prikaz",
                   [this, &pr]{ return asyncFunc().async_update(pr, t_prikaz_name); });

}
#endif

#if 0
qx::dao::ptr<t_ttno> GrainShipmentCulture::ttnWithMaxNetto() throw (MainSequenceException)
{

    const QString q = "select * from t_ttno where brutto - tara = " \
       "(select MAX(brutto-tara) from t_ttno where car=" + QString::number(current_car->id) +
       " AND num_nakl != " + QString::number(current_ttn->num_nakl) + " and brutto > 0  LIMIT 0,1)";

    qx::dao::ptr<t_ttno> ttn_max_netto = wrap_async_ex( tr(cant_get_ttn_with_max_netto),
       "cant get ttn with max netto for car: " + QString::number(current_car->id),
        [this, &q]{return asyncFunc().async_exec_query<t_ttno>(q, false); }  );

    return ttn_max_netto;


    return qx::dao::ptr<t_ttno> ();
}
#endif

#if 0
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

    return 0;
}
#endif


int GrainShipmentCulture::getVirtualNetto()
{
#if 0
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

    return 0;

#endif

    auto t1 = ttn_table.as("t1");
    auto t2 = ttn_table.as("t2");
    auto netto = sql::alias<_S("netto")>(t1.brutto - t1.tara);
    auto netto_sum = sql::alias<_S("netto_sum")>(netto);

    auto q = sql::select( sql::sum(netto) ).from (
                sql::select( sql::max( t1.brutto - t1.tara ) ).as( netto ).from( t1 ).
                    inner_join( t2 ).on( t1.car == t2.car ).
                        where( ((t2.tara != 0 && t2.brutto == 0) || (t2.car == current_car[cars_table.id] )) &&
                               (t1.brutto > t1.tara) && (t1.prikaz == current_prikaz[prikaz_table.num_nakl]) )
                        .group_by( t1.car )

                ).as(netto_sum);

    int virtual_netto = async2().fetch(q, tr(cant_get_virtual_netto) );
    return virtual_netto;
}



void GrainShipmentCulture::checkVirtualNetto()
{
    int vn = getVirtualNetto();

    current_prikaz[ prikaz_table.virtual_rest ] = vn;

    if ( current_prikaz[prikaz_table.rest] + vn > current_prikaz[prikaz_table.lim] )
        throw MainSequenceException(tr(car_netto_too_big_for_prikaz),
            "car " + QString::number( current_car[cars_table.id] ) +
            " by ttn: " + QString::number(current_ttn[ttn_table.num_nakl]) +
            " prikaz: " + QString::number( current_prikaz[prikaz_table.num_nakl] ) +
            " virtual rest: " + QString::number(vn) +
            " limit: " + QString::number( current_prikaz[prikaz_table.rest] ));


}


} }

#undef _S
