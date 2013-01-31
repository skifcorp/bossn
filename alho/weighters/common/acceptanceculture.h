#ifndef ACCEPTANCE_CULTURE_H
#define ACCEPTANCE_CULTURE_H

#include "culture.h"
#include "mainsequence.h"
#include "func.h"

#include "qxorm_pch.h"

namespace alho { namespace common {

namespace sql = boost::rdb::mysql;

class AcceptanceCulture : public Culture
{
public:    
    AcceptanceCulture(MainSequence & as, QSqlDatabase& db, boost::rdb::mysql::mysql_database& db2) : Culture(as, db, db2){}
#if 0
    template <class Tttn>
    qx::dao::ptr<Tttn> ttnByDriver( int drv ) throw (MainSequenceException)
	{
		auto ret = wrap_async_ex(tr(get_ttn_by_driver_tara0_error_message), "getting ttn by driver with zero tara failed: " + QString::number(drv),
                            [&drv, this]{ return asyncFunc().async_exec_query<Tttn>(
                                                 "select * from t_ttn where driver="+QString::number(drv) + " and brutto != 0 and tara=0", false);});

		if ( ret ) {
            seq().seqWarning() << "Corrupted data on card!. Maybe dispatcher made task before tara finished! drv: " + QString::number(drv) <<" founded ttn: " + QString::number(ret->num_nakl);
			return ret;
		}

		const QString qq = "select * from t_ttn where driver = " + QString::number(drv)
			+ " and dt_of_tara = (select MAX(dt_of_tara) from t_ttn where driver = " + QString::number(drv) + " limit 0, 1 )";


		ret = wrap_async_ex(tr(get_last_ttn_by_driver_error_message), "getting last ttn by driver failed! drv: " + QString::number(drv),
                            [&drv, this, &qq]{ return asyncFunc().async_exec_query<Tttn>(qq);});

		return ret;	
	}
#endif
    template <class TtnData, class TtnTable>
    TtnData ttnByDriver2( int drv, const TtnTable& tt )
    {
#if 0
        auto ret = wrap_async_ex(tr(get_ttn_by_driver_tara0_error_message), "getting ttn by driver with zero tara failed: " + QString::number(drv),
                            [&drv, this]{ return asyncFunc().async_exec_query<Tttn>(
                                                 "select * from t_ttn where driver="+QString::number(drv) + " and brutto != 0 and tara=0", false);});

        if ( ret ) {
            seq().seqWarning() << "Corrupted data on card!. Maybe dispatcher made task before tara finished! drv: " + QString::number(drv) <<" founded ttn: " + QString::number(ret->num_nakl);
            return ret;
        }

        const QString qq = "select * from t_ttn where driver = " + QString::number(drv)
            + " and dt_of_tara = (select MAX(dt_of_tara) from t_ttn where driver = " + QString::number(drv) + " limit 0, 1 )";


        ret = wrap_async_ex(tr(get_last_ttn_by_driver_error_message), "getting last ttn by driver failed! drv: " + QString::number(drv),
                            [&drv, this, &qq]{ return asyncFunc().async_exec_query<Tttn>(qq);});

        return ret;
#endif
        auto q1 = sql::select( tt.all ).from( tt ).where( tt.driver == drv && tt.brutto != 0 && tt.tara == 0 );
        auto deq = async2().exec( q1, tr(get_ttn_by_driver_tara0_error_message) ).all();
        if ( deq.size() > 0 ) {
            seq().seqWarning() << "Corrupted data on card!. Maybe dispatcher made task before tara finished! drv: "
                                  + QString::number(drv) <<" founded ttn: " + QString::number( deq.front()[tt.num_nakl] );
            return deq.front();
        }

        auto q2 = sql::select( sql::max( tt.dt_of_tara ), tt.all ).from(tt).where(tt.driver == drv);

        auto ttn = async2().fetch( q2, tr(get_last_ttn_by_driver_error_message) );

        return std::move(  erase_nullable( ttn, sql::max( tt.dt_of_tara ) )  );
    }

#if 0
    template <class Tttn, class Tcars>
    void processDrivingTime(qx::dao::ptr<Tttn> ttn, qx::dao::ptr<Tcars> car )const throw (MainSequenceException)
	{
		ushort rup      = dateTimeToTimeShit(ttn->time_return) - dateTimeToTimeShit(ttn->date_time) / 60; /*to hours*/

		if ( car->vremja_na_hodku > rup ) {
			ttn->time_return = timeShitToDateTime( dateTimeToTimeShit(ttn->date_time) + car->vremja_na_hodku * 60 );
		}	
	}
#endif

    template <class TtnTable, class TtnData, class CarsTable, class CarsData>
    void processDrivingTime2(const TtnTable& tt, TtnData & ttn, const CarsTable& ct, const CarsData& car )const
    {
        ushort rup      = dateTimeToTimeShit( ptime_to_qt(ttn[tt.time_return]) ) -
                                dateTimeToTimeShit( ptime_to_qt(ttn[tt.date_time]) ) / 60; /*to hours*/

        if ( car[ct.vremja_na_hodku] > rup ) {
            ttn[tt.time_return] =  boost::posix_time::from_time_t(
                                    timeShitToDateTime( dateTimeToTimeShit( ptime_to_qt(ttn[tt.date_time]) )
                                         + car[ct.vremja_na_hodku] * 60 ).toTime_t() );
        }
    }

#if 0
    template<class Tttn, class Tfield>
    void repairFieldCodeCorrectnessIfNeeded(MifareCardData &bill, qx::dao::ptr<Tttn> ttn,
                                            const QString& table_name)  throw()
	{
		if ( bill.memberValue<uint>("realNumField") == 0 || !wrap_async( [&bill, this, &table_name]{
                return asyncFunc().async_fetch<Tfield>( bill.memberValue<uint>("realNumField"), table_name );
			} ) )  {

			bill.setMemberValue("realNumField", bill.memberValue<uint>("numField"));
			ttn->real_field = bill.memberValue<uint>("numField");
		}	
	}
#endif
    template<class Tttn, class Tttn_data, class Tfield>
    void repairFieldCodeCorrectnessIfNeeded2(MifareCardData &bill,
        const Tfield& field_table, const Tttn& ttn_table, Tttn_data & ttn_data)
    {
        auto q = sql::select( sql::count() ).from(field_table).where( field_table.id ==
                                                                      bill.memberValue<int>("realNumField") );       

        if ( bill.memberValue<uint>("realNumField") == 0 || async2().fetch( q, "fetching field_table failed!") )
        {
            bill.setMemberValue("realNumField", bill.memberValue<int>("numField"));

            //ttn_data.replace( ttn_table.real_field, bill.memberValue<int>("numField") );

            //static_name_of < decltype(ttn_data[ ttn_table.real_field ]) > aaa;

            //ttn_data[ ttn_table.real_field ] = bill.memberValue<int>("numField");



            qDebug() << "INFO: t_ttn: " << ttn_data[ttn_table.num_nakl] << " realNumField: " << bill.memberValue<uint>("realNumField")
                     << " will be changed to " << bill.memberValue<int>("numField");
        }
    }

#if 0
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

        auto q = sql::select( sql::avg( tt.tara  ) ).from( tt ).where( tt.tara != 0 && tt.num_nakl != ttn[tt.num_nakl] &&
                tt.car  == car[ct.id]).order_by( -tt.dt_of_tara ).limit(count);

        int mid_tara = async2().fetch( q, tr(error_getting_mid_tara_message) );

        ttn[tt.rup_tara]           = mid_tara * percent / 100;
        ttn[tt.real_rup_tara]      = qAbs(mid_tara - static_cast<int>(ttn[tt.tara]));


    }
  
};


} }

#endif

