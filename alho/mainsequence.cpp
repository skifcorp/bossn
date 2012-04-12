#include "mainsequence.h"
#include "mifarecard.h"
#include "settingstool.h"
#include "func.h"

const StructConf & bill_conf(const QVariantMap& opts)
{
    static StructConf s;

    if (!s.empty()) return s;

    uint data_block = opts["data_block"].toUInt();

    s.blocks.append(0 + data_block);
    s.blocks.append(1 + data_block);
    s.blocks.append(2 + data_block);

    s.members_conf.append(StructMemberConf("billNumber"    , "uint"   , 0, 3));
    s.members_conf.append(StructMemberConf("flags"         , "boolarr", 3, 2));
    s.members_conf.append(StructMemberConf("numField"      , "ushort" , 5, 2));
    s.members_conf.append(StructMemberConf("realNumField"  , "ushort" , 7, 2));
    s.members_conf.append(StructMemberConf("numLoader"     , "uchar"  , 9, 1));
    s.members_conf.append(StructMemberConf("dateOfLoad"    , "datetimeshit" , 10, 3));
    s.members_conf.append(StructMemberConf("driver"        , "ushort"   , 13, 2));
    s.members_conf.append(StructMemberConf("pointOfAnal"   , "uchar"    , 15, 2));

    s.members_conf.append(StructMemberConf("bruttoWeight"  , "ushort"   , 0 + 16, 2));
    s.members_conf.append(StructMemberConf("dateOfBrutto"  , "datetimeshit" , 2 + 16, 2));
    s.members_conf.append(StructMemberConf("taraWeight"    , "ushort"   , 5 + 16, 2));
    s.members_conf.append(StructMemberConf("normTaraWeight", "ushort"   , 7 + 16, 2));
    s.members_conf.append(StructMemberConf("dateOfTara"    , "datetimeshit" , 9 + 16, 2));

    s.members_conf.append(StructMemberConf("impurity"      , "ushort"   , 0 + 32, 2));
    s.members_conf.append(StructMemberConf("shugarContent" , "ushort"   , 2 + 32, 2));
    s.members_conf.append(StructMemberConf("greenWeight"   , "ushort"   , 4 + 32, 2));
    s.members_conf.append(StructMemberConf("bum"           , "uchar"    , 6 + 32, 1));
    s.members_conf.append(StructMemberConf("bumFact"       , "uchar"    , 7 + 32, 1));
    s.members_conf.append(StructMemberConf("kagat"         , "uchar"    , 8 + 32, 1));
    s.members_conf.append(StructMemberConf("dateOfUnload"  , "datetimeshit" , 9 + 32, 3));

    return s;
}

//QByteArray MainSequence::card_code;



void MainSequence::onAppearOnWeight()
{
    qDebug() << "something appeared on weight!!!!";
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
}


