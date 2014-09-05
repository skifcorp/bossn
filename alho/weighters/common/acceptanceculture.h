#ifndef ACCEPTANCE_CULTURE_H
#define ACCEPTANCE_CULTURE_H

#include "culture.h"
#include "mainsequence.h"
#include "func.h"

#ifdef AAAA

namespace alho { namespace common {

//namespace sql = boost::rdb::mysql;

class AcceptanceCulture : public Culture
{
public:    
    AcceptanceCulture(MainSequence & as, QSqlDatabase& db, ::boost::rdb::mysql::mysql_database& db2) : Culture(as, db, db2){}




    template <class TtnTable, class TtnData, class CarsTable, class CarsData>
    void processDrivingTime2(const TtnTable& tt, TtnData & ttn, const CarsTable& ct, const CarsData& car )const
    {

        ushort rup      = dateTimeToTimeShit( ptime_to_qt(ttn[tt.time_return]) ) -
                                dateTimeToTimeShit( ptime_to_qt(ttn[tt.date_time]) ) / 60; /*to hours*/

        if ( car[ct.vremja_na_hodku] > rup ) {
            ttn[tt.time_return] =  ::boost::posix_time::from_time_t(
                                    timeShitToDateTime( dateTimeToTimeShit( ptime_to_qt(ttn[tt.date_time]) )
                                         + car[ct.vremja_na_hodku] * 60 ).toTime_t() );
        }

    }


    template<class Tttn, class Tttn_data, class Tfield>
    void repairFieldCodeCorrectnessIfNeeded2(MifareCardData &bill,
        const Tfield& field_table, const Tttn& ttn_table, Tttn_data & ttn_data)
    {
        auto q = ::boost::rdb::mysql::select( ::boost::rdb::mysql::count() ).from(field_table).where( field_table.id ==
                                                                      bill.memberValue<int>("realNumField") );       

        if ( bill.memberValue<uint>("realNumField") == 0 || async2().fetch( q, "fetching field_table failed!") )
        {
            bill.setMemberValue("realNumField", bill.memberValue<int>("numField"));

            qDebug() << "INFO: t_ttn: " << ttn_data[ttn_table.num_nakl] << " realNumField: " << bill.memberValue<uint>("realNumField")
                     << " will be changed to " << bill.memberValue<int>("numField");
        }
    }

    template <class TtnTable, class TtnData, class CarsTable, class CarsData>
    void processTaraRupture2(const TtnTable & tt, TtnData& ttn, const CarsTable & ct, const CarsData & car )
    {
        //QString percent = convienceFunc().getConst<Tconst>( seq().appSetting<QString>("tara_percent_name") )->value ;

        int percent = constantValue<int>( seq().appSetting<QString>("tara_percent_name") );

        int count{};
        if ( car[ct.amount_of_car_for_middle_tara] < 0 ) {
            count = constantValue<int>( seq().appSetting<QString>("tara_cars_mid_count_name") ) ;
        }
        else {
            count = car[ct.amount_of_car_for_middle_tara];
        }

        /*int mid_tara = wrap_async_ex( tr(error_getting_mid_tara_message), "error getting mid tara",
                                        [this, &ttn, &car, &count]{  return asyncFunc().async_call_query<int>(
                                            "select avg(tara) from (select tara from t_ttn where tara!=0 and num_nakl!="+
                                            QString::number(ttn->num_nakl)+
                                            " and car="+QString::number(car->id)+
                                            " order by -dt_of_tara limit "+count+") as temp_table;");});*/

        auto q = ::boost::rdb::mysql::select( ::boost::rdb::mysql::avg( tt.tara  ) ).from( tt ).where( tt.tara != 0 && tt.num_nakl != ttn[tt.num_nakl] &&
                tt.car  == car[ct.id]).order_by( -tt.dt_of_tara ).limit(count);

        int mid_tara = async2().fetch( q, tr(error_getting_mid_tara_message) );

        ttn[tt.rup_tara]           = mid_tara * percent / 100;
        ttn[tt.real_rup_tara]      = qAbs(mid_tara - static_cast<int>(ttn[tt.tara]));


    }
  
};


} }

#endif

#endif

