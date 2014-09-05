#ifndef BASEACCEPTANCEWEIGHTER_H
#define BASEACCEPTANCEWEIGHTER_H

#include "weighter.h"
#include "mainsequence.h"
#include "func.h"
#include "reportsmanager.h"

#ifdef AAAA

namespace alho { namespace kryzh {

using namespace alho::common;

template <class C>
class AcceptanceWeighter : public Weighter,
                           protected C
{
public:
    AcceptanceWeighter( MainSequence & as, QSqlDatabase& db, ::boost::rdb::mysql::mysql_database& db2 ):Weighter(), C(as, db, db2){}
    ~AcceptanceWeighter() {}

    virtual void processWeighting(MifareCardData& bill, MifareCardSector& card, const  WeighterConf& weighter_conf )
    {
        C::fillConstants();

        C::fetchCar(bill);

        C::seq().processPerimeter();

        QString platform_type = weighter_conf.platform_type == "auto" ?
                                    C::detectPlatformType(bill) :
                                    weighter_conf.platform_type;

        if (platform_type == "brutto" ) {
            processBrutto(bill);
            card.writeStruct(CardStructs::bill_conf( weighter_conf.weighter_name ), bill,
                             CardStructs::blocks_conf() );

            C::seq().printOnTablo( C::bruttoFinishMessage(bill) );

            //C::seq().alhoSettings().reader.do_sound.func(Q_ARG(QVariant,
            //            C::seq().template appSetting<int>("beep_length")));
            card.readerTagMethods().do_sound.func( Q_ARG(QVariant,
                C::seq().template appSetting<int>("beep_length")) );

        }
        else if (platform_type == "tara" ) {
            bool pure_tara = processTara(bill);
            bool new_ttn = false;

            ReportContext finish_report_context = C::finishReport();
            QString tara_message = C::taraFinishMessage(bill);

            if (pure_tara) {
                new_ttn = C::makeNewTask(bill);
            }

            card.writeStruct(CardStructs::bill_conf( weighter_conf.weighter_name ), bill,
                             CardStructs::blocks_conf());
            C::seq().printOnTablo( tara_message );
            //C::seq().alhoSettings().reader.do_sound.func(Q_ARG(QVariant,
            //                  C::seq().template appSetting<int>("beep_length")));
            card.readerTagMethods().do_sound.func( Q_ARG(QVariant,
                C::seq().template appSetting<int>("beep_length")) );

            ReportsManager rp;
            rp.setPrinterName( C::seq().printerName() );
            rp.printReport( finish_report_context, weighter_conf.member<QString>("finish_report") );


            if (new_ttn)
                rp.printReport( C::startReport(), weighter_conf.member<QString>("start_report") );

        }
    }

private:
    void processBrutto( MifareCardData& bill )
    {
        int weight = C::getWeight();

        C::checkWeightCorrectess( weight );


        if ( !C::isPureBruttoWeight(bill) ){
            C::reBrutto(weight, bill );
        }
        else {
            C::brutto(weight, bill);
        }
    }

    bool processTara(MifareCardData &bill)
    {
        int weight = C::getWeight();

        C::checkWeightCorrectess ( weight );
        bool ret = true;
        if ( !C::isPureTaraWeight(bill) ) {
            C::reTara(weight, bill);

            ret = false;
        }
        else  {
            C::tara(weight, bill);
        }

        return ret;
    }

};



} }

#endif

#endif // BASEACCEPTANCEWEIGHTER_H
