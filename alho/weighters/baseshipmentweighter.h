

#ifndef BASESHIPMENTWEIGHTER_H
#define BASESHIPMENTWEIGHTER_H

#include "baseweighter.h"
#include "mainsequence.h"

class BaseShipmentWeighter : public BaseWeighter
{
public:
    BaseShipmentWeighter(MainSequence & as, QSqlDatabase& db )
        :BaseWeighter(as, db), current_tara_is_pure(false),
          current_brutto_is_pure(false){}

    virtual void processWeighting(MifareCardData&, MifareCardBlock&, const WeighterConf&  ) throw (MainSequenceException, MifareCardException) ;

    virtual ~BaseShipmentWeighter() {}

    //virtual void processWeighting(MifareCardData&, MifareCard&, WeighterConf&  ) throw (MainSequenceException);
    virtual bool makeNewTask(MifareCardData& ) throw (MainSequenceException) = 0;

    virtual void fetchCar(const MifareCardData& ) throw (MainSequenceException) = 0;

    virtual void brutto(int, MifareCardData& ) throw (MainSequenceException) = 0;
    virtual void tara(int, MifareCardData&) throw (MainSequenceException) = 0;

    virtual void reBrutto(int, MifareCardData&) throw (MainSequenceException) = 0;
    virtual void reTara(int, MifareCardData&) throw (MainSequenceException) = 0;

    virtual QString bruttoFinishMessage(const MifareCardData& )const = 0;
    virtual QString taraFinishMessage(const MifareCardData& )const = 0;

    virtual ReportContext finishReport( ) throw(MainSequenceException) = 0;
    virtual ReportContext startReport( ) throw(MainSequenceException) = 0;

    virtual bool isPureBruttoWeight(const MifareCardData& ) const throw (MainSequenceException) = 0;
    virtual bool isPureTaraWeight(const MifareCardData& ) const throw (MainSequenceException) = 0;
protected:
    template <class Tttn>
    qx::dao::ptr<Tttn> ttnByDriver( int drv ) throw (MainSequenceException)
    {
        auto ret = wrap_async_ex(tr(get_ttn_by_driver_tara0_error_message), "getting ttn by driver with zero tara failed: " + QString::number(drv),
                                [&drv, this]{ return asyncFunc()->async_exec_query<Tttn>(
                                                     "select * from t_ttno where driver="+QString::number(drv) + " and tara != 0 and brutto=0", false);});

        if ( ret ) {
            seq.seqWarning() << "Corrupted data on card!. Maybe dispatcher made task before tara finished! drv: " + QString::number(drv) <<" founded ttn: " + QString::number(ret->num_nakl);
            return ret;
        }

        const QString qq = "select * from t_ttno where driver = " + QString::number(drv)
            + " and dt_of_tara = (select MAX(dt_of_tara) from t_ttno where driver = " + QString::number(drv) + " limit 0, 1 )";


        ret = wrap_async_ex(tr(get_last_ttn_by_driver_error_message), "getting last ttn by driver failed! drv: " + QString::number(drv),
                                [&drv, this, &qq]{ return asyncFunc()->async_exec_query<Tttn>(qq);});

        return ret;
    }
    template <class Tttn, class Tcars, class Tconst>
    void processTaraRupture(qx::dao::ptr<Tttn> ttn, qx::dao::ptr<Tcars> car )  throw (MainSequenceException)
    {
        QString percent = convienceFunc()->getConst<Tconst>( seq.appSetting<QString>("tara_percent_name") )->value ;

        QString count;
        if ( car->amount_of_car_for_middle_tara < 0 ) {
            count = convienceFunc()->getConst<Tconst>( seq.appSetting<QString>("tara_cars_mid_count_name") )->value ;
        }
        else {
            count = QString::number( car->amount_of_car_for_middle_tara );
        }


        int mid_tara = wrap_async_ex( tr(error_getting_mid_tara_message), "error getting mid tara",
                                    [this, &ttn, &car, &count]{  return asyncFunc()->async_call_query<int>(
                                        "select avg(tara) from (select tara from t_ttn where tara!=0 and num_nakl!="+
                                        QString::number(ttn->num_nakl)+
                                        " and car="+QString::number(car->id)+
                                        " order by -dt_of_tara limit "+count+") as temp_table;");});

        ttn->rup_tara           = mid_tara * percent.toUInt() / 100;
        ttn->real_rup_tara      = qAbs(mid_tara - static_cast<int>(ttn->tara));
    }
private:
    bool processBrutto( MifareCardData& ) throw(MainSequenceException);
    void processTara(MifareCardData &) throw(MainSequenceException);

    QString current_platform_type;
    bool current_tara_is_pure;
    bool current_brutto_is_pure;
    QString platformType(const MifareCardData&, const WeighterConf& wc ) throw (MainSequenceException);



};


#endif // BASESHIPMENTWEIGHTER_H
