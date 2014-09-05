

#ifndef BASESHIPMENTWEIGHTER_H
#define BASESHIPMENTWEIGHTER_H

#include "weighter.h"
#include "mainsequence.h"
#include "reportsmanager.h"


#ifdef AAAA

namespace alho { namespace kryzh {

using namespace alho::common;

template <class C>
class ShipmentWeighter : public Weighter,
                         public C
{
public:
    ShipmentWeighter(MainSequence & as, QSqlDatabase& db, ::boost::rdb::mysql::mysql_database& db2 )
        :Weighter(), C(as, db, db2) ,
          current_tara_is_pure(false),
          current_brutto_is_pure(false){}

    virtual void processWeighting(MifareCardData& bill,
                                  MifareCardSector& card, const WeighterConf& weighter_conf  )
    {
        C::fetchCar(bill);

        C::seq().processPerimeter();

        QString platform_type = platformType(bill, weighter_conf);

        if (platform_type == "brutto" ) {
            bool pure_brutto = processBrutto(bill);

            ReportContext finish_report_context = C::finishReport();
            QString brutto_message              = C::bruttoFinishMessage(bill);

            if (pure_brutto) {
               C::makeNewTask(bill);
            }

            card.writeStruct(CardStructs::bill_conf( weighter_conf.weighter_name ), bill,
                             CardStructs::blocks_conf() );

            C::seq().printOnTablo( brutto_message );

            //C::seq().alhoSettings().reader.do_sound.func(Q_ARG(QVariant,
            //                   C::seq().template appSetting<int>("beep_length")));

            card.readerTagMethods().do_sound.func( Q_ARG(QVariant,
                C::seq().template appSetting<int>("beep_length")) );

            ReportsManager rm;
            rm.printReport(finish_report_context, weighter_conf.member<QString>("finish_report_name") );
            rm.printReport(finish_report_context, weighter_conf.member<QString>("finish_report_name") );
        }
        else if (platform_type == "tara" ) {
            processTara(bill);

            card.writeStruct(CardStructs::bill_conf( weighter_conf.weighter_name ), bill,
                             CardStructs::blocks_conf());
            //C::seq().alhoSettings().reader.do_sound.func(Q_ARG(QVariant,
            //                  C::seq().template appSetting<int>("beep_length")));
            card.readerTagMethods().do_sound.func( Q_ARG(QVariant,
                C::seq().template appSetting<int>("beep_length")) );


            C::seq().printOnTablo( C::taraFinishMessage(bill) );
        }

    }

    virtual ~ShipmentWeighter()
    {

    }
 
private:
    bool processBrutto( MifareCardData& bill)
    {
        int weight = C::getWeight();

        C::checkWeightCorrectess( weight );

        bool ret = true;

        if ( !C::isPureBruttoWeight(bill) ){
            if ( current_platform_type == "brutto" && current_brutto_is_pure) {
                C::reBrutto(weight, bill );
            }
            else {
                throw MainSequenceException(C::tr(cant_rebrutto_after_leaving_weights),
                    "Cant rebrutto after leaving weights!!: " +
                       bill.memberValue<QString>("billNumber"));

            }
            ret = false;
        }
        else {
            C::brutto(weight, bill);
            current_brutto_is_pure = true;
        }
        return ret;
    }

    void processTara(MifareCardData & bill)
    {
        int weight = C::getWeight();

        C::checkWeightCorrectess ( weight );



        if ( !C::isPureTaraWeight(bill) ) {
            if ( current_platform_type == "tara" && current_tara_is_pure)
                C::reTara(weight, bill);
            else
                throw MainSequenceException(C::tr(cant_retara_after_leaving_weights),
                    "Cant retara after leaving weights!!: " +
                       bill.memberValue<QString>("billNumber"));

        }
        else  {
            current_tara_is_pure = true;

            C::tara(weight, bill);
        }
    }

    QString current_platform_type;
    bool current_tara_is_pure;
    bool current_brutto_is_pure;

    QString platformType(const MifareCardData& bill, const WeighterConf& weighter_conf )
    {        
        QString ret = weighter_conf.platform_type == "auto" ?
                                    C::detectPlatformType(bill) :
                                    weighter_conf.platform_type;

        if ( current_platform_type.isEmpty() )
            current_platform_type = ret;

        return ret;
    }
};



} }

#endif

#endif // BASESHIPMENTWEIGHTER_H
