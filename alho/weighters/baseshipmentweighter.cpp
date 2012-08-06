#include "baseshipmentweighter.h"
#include "mainsequence.h"

QString BaseShipmentWeighter::platformType(const MifareCardData& bill,
                                           const WeighterConf& wc) throw (MainSequenceException)
{
    QString ret = BaseWeighter::platformType(bill, wc);

    if ( current_platform_type.isEmpty() )
        current_platform_type = ret;

    return ret;
}

void BaseShipmentWeighter::processWeighting(MifareCardData& bill,
                             MifareCard& card, const WeighterConf& weighter_conf) throw (MainSequenceException, MifareCardException)
{
    //qDebug() << "11111";

    fetchCar(bill);

    //qDebug() << "222222";

    QString platform_type = platformType(bill, weighter_conf);

    //qDebug() << "333333: " << platform_type;

    if (platform_type == "brutto" ) {
        bool pure_brutto = processBrutto(bill);

        bool new_ttn = false;

        ReportContext finish_report_context = finishReport();
        QString brutto_message              = bruttoFinishMessage(bill);

        if (pure_brutto) {
            new_ttn = makeNewTask(bill);
        }

        card.writeStruct(CardStructs::bill_conf( weighter_conf.weighter_name ), bill,
                         CardStructs::blocks_conf() );

        seq.printOnTablo( brutto_message );

        seq.alhoSettings().reader.do_sound.func(Q_ARG(QVariant,
                           seq.appSetting<int>("beep_length")));

        ReportsManager rm;
        rm.printReport(finish_report_context, weighter_conf.member<QString>("finish_report_name") );
        rm.printReport(finish_report_context, weighter_conf.member<QString>("finish_report_name") );

//        ReportContext fake_report = fakeReport();
//        if (!fake_report.isEmpty())
//            rm.printReport(fake_report, weighter_conf.member<QString>("new_order_report_name"));

//        ReportContext start_report = startReport();
//        if (!start_report.isEmpty())
//            rm.printReport(start_report, weighter_conf.member<QString>("start_report_name"));

    }
    else if (platform_type == "tara" ) {
        processTara(bill);

        card.writeStruct(CardStructs::bill_conf( weighter_conf.weighter_name ), bill,
                         CardStructs::blocks_conf());
        seq.alhoSettings().reader.do_sound.func(Q_ARG(QVariant,
                          seq.appSetting<int>("beep_length")));


        seq.printOnTablo( taraFinishMessage(bill) );
    }

}


bool BaseShipmentWeighter::processBrutto( MifareCardData& bill ) throw(MainSequenceException)
{
    int weight = getWeight();

    checkWeightCorrectess( weight );

    bool ret = true;

    if ( !isPureBruttoWeight(bill) ){
        if ( current_platform_type == "brutto" && current_brutto_is_pure) {
            reBrutto(weight, bill );
        }
        else {
            throw MainSequenceException(tr(cant_rebrutto_after_leaving_weights),
                "Cant rebrutto after leaving weights!!: " +
                   bill.memberValue<QString>("billNumber"));

        }
        ret = false;
    }
    else {
        brutto(weight, bill);
        current_brutto_is_pure = true;
    }
    return ret;
}

void BaseShipmentWeighter::processTara(MifareCardData & bill) throw(MainSequenceException)
{
    int weight = getWeight();

    checkWeightCorrectess ( weight );



    if ( !isPureTaraWeight(bill) ) {
        if ( current_platform_type == "tara" && current_tara_is_pure)
            reTara(weight, bill);
        else
            throw MainSequenceException(tr(cant_retara_after_leaving_weights),
                "Cant retara after leaving weights!!: " +
                   bill.memberValue<QString>("billNumber"));

    }
    else  {
        current_tara_is_pure = true;

        tara(weight, bill);
    }

}
