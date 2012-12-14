#ifndef BASEACCEPTANCEWEIGHTER_H
#define BASEACCEPTANCEWEIGHTER_H

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
    AcceptanceWeighter( MainSequence & as, QSqlDatabase& db ):Weighter(), C(as, db){}
    ~AcceptanceWeighter() {}

    virtual void processWeighting(MifareCardData& bill, MifareCardSector& card, const  WeighterConf& weighter_conf )
    {
        C::fetchCar(bill);
        C::checkPerimetr();
        Q_ASSERT(0);
        QString platform_type;// = C::platformType(bill, weighter_conf);


        if (platform_type == "brutto" ) {
            processBrutto(bill);
            card.writeStruct(CardStructs::bill_conf( weighter_conf.weighter_name ), bill,
                             CardStructs::blocks_conf() );

            C::seq().printOnTablo( C::bruttoFinishMessage(bill) );

            C::seq().alhoSettings().reader.do_sound.func(Q_ARG(QVariant,
                        C::seq().template appSetting<int>("beep_length")));
        }
        else if (platform_type == "tara" ) {
            processTara(bill);
            bool new_ttn = false;

            
            QString tara_message = C::taraFinishMessage(bill);

            card.writeStruct(CardStructs::bill_conf( weighter_conf.weighter_name ), bill,
                             CardStructs::blocks_conf());
            C::seq().printOnTablo( tara_message );
            C::seq().alhoSettings().reader.do_sound.func(Q_ARG(QVariant,
                              C::seq().template appSetting<int>("beep_length")));

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


#endif // BASEACCEPTANCEWEIGHTER_H
