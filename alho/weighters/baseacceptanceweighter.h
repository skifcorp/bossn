#ifndef BASEACCEPTANCEWEIGHTER_H
#define BASEACCEPTANCEWEIGHTER_H

#include "baseweighter.h"
#include "mainsequence.h"


class BaseAcceptanceWeighter : public BaseWeighter
{
public:
    BaseAcceptanceWeighter( MainSequence & as, QSqlDatabase& db ):BaseWeighter(as, db){}
    virtual ~BaseAcceptanceWeighter() {}


    virtual void fetchCar(const MifareCardData& ) throw (MainSequenceException) = 0;

    virtual void brutto(int, MifareCardData& ) throw (MainSequenceException) = 0;
    virtual void tara(int, MifareCardData&) throw (MainSequenceException) = 0;

    virtual void reBrutto(int, MifareCardData&) throw (MainSequenceException) = 0;
    virtual void reTara(int, MifareCardData&) throw (MainSequenceException) = 0;

    virtual bool makeNewTask(MifareCardData& ) throw (MainSequenceException) = 0;

    virtual QString bruttoFinishMessage(const MifareCardData& )const = 0;
    virtual QString taraFinishMessage(const MifareCardData& )const = 0;

    virtual ReportContext finishReport( ) throw(MainSequenceException) = 0;
    virtual ReportContext startReport( ) throw(MainSequenceException) = 0;

    virtual bool isPureBruttoWeight(const MifareCardData& ) const throw (MainSequenceException) = 0;
    virtual bool isPureTaraWeight(const MifareCardData& ) const throw (MainSequenceException) = 0;


    virtual void processWeighting(MifareCardData&, MifareCardBlock&, const  WeighterConf&  ) throw (MainSequenceException, MifareCardException);
protected:

    template <class Tttn>
    qx::dao::ptr<Tttn> ttnByDriver( int drv ) throw (MainSequenceException);
    //template <class Tttn, class Tcars, class Tconst>
    //void processTaraRupture(qx::dao::ptr<Tttn> ttn, qx::dao::ptr<Tcars> car )  throw (MainSequenceException);
    template <class Tttn, class Tcars>
    void processDrivingTime(qx::dao::ptr<Tttn> ttn, qx::dao::ptr<Tcars> car )const throw (MainSequenceException);
    template<class Tttn, class Tfield>
    void repairFieldCodeCorrectnessIfNeeded(MifareCardData &bill, qx::dao::ptr<Tttn> ttn,
                                            const QString& table_name)  throw();
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
        if ( count.isEmpty() ) {
            qDebug() << "mid_count_of_cars is empty!!!!";
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
    //QString platformType(const MifareCardData& ) const throw (MainSequenceException);

    void processBrutto( MifareCardData& ) throw(MainSequenceException);
    bool processTara(MifareCardData &) throw(MainSequenceException);


};





template <class Tttn>
qx::dao::ptr<Tttn> BaseAcceptanceWeighter::ttnByDriver( int drv ) throw (MainSequenceException)
{
    auto ret = wrap_async_ex(tr(get_ttn_by_driver_tara0_error_message), "getting ttn by driver with zero tara failed: " + QString::number(drv),
                            [&drv, this]{ return asyncFunc()->async_exec_query<Tttn>(
                                                 "select * from t_ttn where driver="+QString::number(drv) + " and brutto != 0 and tara=0", false);});

    if ( ret ) {
        seq.seqWarning() << "Corrupted data on card!. Maybe dispatcher made task before tara finished! drv: " + QString::number(drv) <<" founded ttn: " + QString::number(ret->num_nakl);
        return ret;
    }

    const QString qq = "select * from t_ttn where driver = " + QString::number(drv)
        + " and dt_of_tara = (select MAX(dt_of_tara) from t_ttn where driver = " + QString::number(drv) + " limit 0, 1 )";


    ret = wrap_async_ex(tr(get_last_ttn_by_driver_error_message), "getting last ttn by driver failed! drv: " + QString::number(drv),
                            [&drv, this, &qq]{ return asyncFunc()->async_exec_query<Tttn>(qq);});

    return ret;
}
/*
template <class Tttn, class Tcars, class Tconst>
void BaseAcceptanceWeighter::processTaraRupture(qx::dao::ptr<Tttn> ttn, qx::dao::ptr<Tcars> car )  throw (MainSequenceException)
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
*/
template <class Tttn, class Tcars>
void BaseAcceptanceWeighter::processDrivingTime(qx::dao::ptr<Tttn> ttn, qx::dao::ptr<Tcars> car )const throw (MainSequenceException)
{
    ushort rup      = dateTimeToTimeShit(ttn->time_return) - dateTimeToTimeShit(ttn->date_time) / 60; /*to hours*/

    if ( car->vremja_na_hodku > rup ) {
        ttn->time_return = timeShitToDateTime( dateTimeToTimeShit(ttn->date_time) + car->vremja_na_hodku * 60 );
    }

}

template<class Tttn, class Tfield>
void BaseAcceptanceWeighter::repairFieldCodeCorrectnessIfNeeded(MifareCardData &bill, qx::dao::ptr<Tttn> ttn,
                                        const QString& table_name)  throw()
{
    if ( bill.memberValue<uint>("realNumField") == 0 || !wrap_async( [&bill, this, &table_name]{
           return asyncFunc()->async_fetch<Tfield>( bill.memberValue<uint>("realNumField"), table_name );
        } ) )  {

        bill.setMemberValue("realNumField", bill.memberValue<uint>("numField"));
        ttn->real_field = bill.memberValue<uint>("numField");
    }

}



#endif // BASEACCEPTANCEWEIGHTER_H
