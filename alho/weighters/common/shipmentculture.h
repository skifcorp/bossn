#ifndef KRYZH_CULTURE
#define KRYZH_CULTURE

#include "culture.h"
#include "mainsequence.h"

namespace alho { namespace common {

class ShipmentCulture : public Culture
{
public:
    ShipmentCulture(MainSequence & as, QSqlDatabase& db, ::boost::rdb::mysql::mysql_database& db2) : Culture(as, db, db2){}
#if 0
    template <class Tttn>
    qx::dao::ptr<Tttn> ttnByDriver( int drv ) throw (MainSequenceException)
	{   
        auto ret = wrap_async_ex(tr(get_ttn_by_driver_tara0_error_message), "getting ttn by driver with zero tara failed: " + QString::number(drv),
                                [&drv, this]{ return asyncFunc().async_exec_query<Tttn>(
                                                     "select * from t_ttno where driver="+QString::number(drv) + " and tara != 0 and brutto=0", false);});

        if ( ret ) {
            seq().seqWarning() << "Corrupted data on card!. Maybe dispatcher made task before tara finished! drv: " + QString::number(drv) <<" founded ttn: " + QString::number(ret->num_nakl);
            return ret;
        }

        const QString qq = "select * from t_ttno where driver = " + QString::number(drv)
            + " and dt_of_tara = (select MAX(dt_of_tara) from t_ttno where driver = " + QString::number(drv) + " limit 0, 1 )";


        ret = wrap_async_ex(tr(get_last_ttn_by_driver_error_message), "getting last ttn by driver failed! drv: " + QString::number(drv),
                                [&drv, this, &qq]{ return asyncFunc().async_exec_query<Tttn>(qq);});

        return ret;    
	}
    template <class Tttn, class Tcars>
    void processDrivingTime(qx::dao::ptr<Tttn> ttn, qx::dao::ptr<Tcars> car )const throw (MainSequenceException);
    template<class Tttn, class Tfield>
    void repairFieldCodeCorrectnessIfNeeded(MifareCardData &bill, qx::dao::ptr<Tttn> ttn,
                                            const QString& table_name)  throw();
    template <class Tttn, class Tcars, class Tconst>
    void processTaraRupture(qx::dao::ptr<Tttn> ttn, qx::dao::ptr<Tcars> car )  throw (MainSequenceException)
    {
        QString percent = convienceFunc().getConst<Tconst>( seq().appSetting<QString>("tara_percent_name") )->value ;

        QString count;
        if ( car->amount_of_car_for_middle_tara < 0 ) {
            count = convienceFunc().getConst<Tconst>( seq().appSetting<QString>("tara_cars_mid_count_name") )->value ;
        }
        else {
            count = QString::number( car->amount_of_car_for_middle_tara );
        }
        if ( count.isEmpty() ) {
            qDebug() << "mid_count_of_cars is empty!!!!";
        }

        int mid_tara = wrap_async_ex( tr(error_getting_mid_tara_message), "error getting mid tara",
                                        [this, &ttn, &car, &count]{  return asyncFunc().async_call_query<int>(
                                            "select avg(tara) from (select tara from t_ttn where tara!=0 and num_nakl!="+
                                            QString::number(ttn->num_nakl)+
                                            " and car="+QString::number(car->id)+
                                            " order by -dt_of_tara limit "+count+") as temp_table;");});

         ttn->rup_tara           = mid_tara * percent.toUInt() / 100;
         ttn->real_rup_tara      = qAbs(mid_tara - static_cast<int>(ttn->tara));
    }
#endif
};


} }

#endif

