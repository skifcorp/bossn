#include "mainsequence.h"
#include "mifarecard.h"
#include "settingstool.h"
#include "func.h"


//QByteArray MainSequence::card_code;


MainSequence::MainSequence(Tags & t, const QVariantMap& opts):tags(t), options(opts), on_weight(false)
{
    tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant("Zaidjte na vagu")));
}

void MainSequence::onAppearOnWeight()
{
    qDebug() << "something appeared on weight!!!!";
    tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant("Pidnesith kartku")));

    //tags["tablo"]->func("value", Q_ARG(QVariant, "Hello"));
    //need turn on red and off green and yellow

    //tags["do3"]->func("writeMethod", Q_ARG(QVariant, true));
    tags["reader1"]->func("doOn");
    on_weight = true;
    QByteArray card_code;
    if (!get_setting("card_code", options, card_code) ) { return; }

    uint data_block = 128;
    if (!get_setting("data_block", options, data_block) ) { return; }

    uint sleepnb_timeout = 100;
    get_setting("sleepnb_timeout", options, sleepnb_timeout);

    while(on_weight) {
        ActivateCardISO14443A act = tags["reader1"]->func("activateIdleA").value<ActivateCardISO14443A>();
        MifareCard card(tags["reader1"], act);

        if ( !card.active() ) {
            qDebug() << "card not active!!!";

            sleepnb(sleepnb_timeout); continue;
        }

        qDebug()<< "card active!";

        if ( !card.autorize(card_code, data_block) ) {
            qDebug() << "fail to autorize!!!!";

            sleepnb(sleepnb_timeout); continue;
        }


        QVariantMap bill = card.readStruct(bill_conf(options));
        if ( bill.isEmpty() ) {
            qDebug() << "cant read bill!!!!";

            sleepnb(sleepnb_timeout); continue;
        }

        if (!checkMember("billNumber", bill, 0) || !checkMember("driver", bill, 0)) {
            qDebug() << "bill is empty!!!";

            sleepnb(sleepnb_timeout); continue;
        }



        sleepnb(sleepnb_timeout);
    }

}


void MainSequence::onDisappearOnWeight()
{
    qDebug() << "something disappeared on weight!!!!";
    //tags["tablo"]->func("value", Q_ARG(QVariant,"Bye"));
    //tags["tablo"]->func("value", Q_ARG(QVariant, "Hello"));
    //need turn on red and off green and yellow
    //tags["do1"]->func("writeValue", Q_ARG(QVariant, true));
    on_weight = false;

    tags["reader1"]->func("doOff");
    tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant("Zaidkte na vagu")));
}


