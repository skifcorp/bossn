#include "baseacceptanceweighter.h"
#include "mainsequence.h"


void BaseAcceptanceWeighter::processWeighting(MifareCardData& bill, MifareCard& card,
                const WeighterConf& weighter_conf ) throw ( MainSequenceException , MifareCardException)
{

    fetchCar(bill);

    QString platform_type = platformType(bill, weighter_conf);


    if (platform_type == "brutto" ) {
        processBrutto(bill);
        card.writeStruct(CardStructs::bill_conf( weighter_conf.weighter_name ), bill,
                         CardStructs::blocks_conf() );

        seq.printOnTablo( bruttoFinishMessage(bill) );

        seq.alhoSettings().reader.do_sound.func(Q_ARG(QVariant,
                           seq.appSetting<int>("beep_length")));
    }
    else if (platform_type == "tara" ) {
        bool pure_tara = processTara(bill);
        bool new_ttn = false;

        ReportContext finish_report_context = finishReport();
        QString tara_message = taraFinishMessage(bill);

        if (pure_tara) {
            new_ttn = makeNewTask(bill);
        }

        card.writeStruct(CardStructs::bill_conf( weighter_conf.weighter_name ), bill,
                         CardStructs::blocks_conf());
        seq.printOnTablo( tara_message );
        seq.alhoSettings().reader.do_sound.func(Q_ARG(QVariant,
                          seq.appSetting<int>("beep_length")));

        ReportsManager rp;
        rp.setPrinterName( seq.printerName() );
        rp.printReport( finish_report_context, weighter_conf.member<QString>("finish_report") );


        if (new_ttn)
            rp.printReport( startReport(), weighter_conf.member<QString>("start_report") );


    }
}



void BaseAcceptanceWeighter::processBrutto( MifareCardData& bill) throw(MainSequenceException)
{
    int weight = getWeight();

    checkWeightCorrectess( weight );


    if ( !isPureBruttoWeight(bill) ){
        reBrutto(weight, bill );
    }
    else {
        brutto(weight, bill);
    }
}

bool BaseAcceptanceWeighter::processTara(MifareCardData & bill) throw(MainSequenceException)
{
    int weight = getWeight();

    checkWeightCorrectess ( weight );
    bool ret = true;
    if ( !isPureTaraWeight(bill) ) {
        reTara(weight, bill);

        ret = false;
    }
    else  {
        tara(weight, bill);
    }

    return ret;
}
