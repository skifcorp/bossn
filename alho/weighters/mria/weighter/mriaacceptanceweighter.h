#ifndef MRIAACCEPTANCEWEIGHTER_H
#define MRIAACCEPTANCEWEIGHTER_H

#ifdef AAAA

#include "weighter.h"
#include "mainsequence.h"
#include "func.h"
#include "reportsmanager.h"



namespace alho { namespace mria {

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
        C::fetchCar(bill);
        C::checkPerimetr();

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
            processTara(bill);
            
            QString tara_message = C::taraFinishMessage(bill);

            card.writeStruct(CardStructs::bill_conf( weighter_conf.weighter_name ), bill,
                             CardStructs::blocks_conf());
            C::seq().printOnTablo( tara_message );
//            C::seq().alhoSettings().reader.do_sound.func(Q_ARG(QVariant,
//                              C::seq().template appSetting<int>("beep_length")));
            card.readerTagMethods().do_sound.func( Q_ARG(QVariant,
                C::seq().template appSetting<int>("beep_length")) );

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
