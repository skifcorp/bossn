#include "mainsequence.h"
#include "mifarecard.h"


const StructConf & bill_conf()
{
    static StructConf s;

    if (!s.empty()) return s;

    s.sectors.append(0 + 128);
    s.sectors.append(1 + 128);
    s.sectors.append(2 + 128);

    s.members_conf.append(StructMemberConf("billNumber"    , "uint"   , 0, 3));
    s.members_conf.append(StructMemberConf("flags"         , "boolarr", 3, 2));
    s.members_conf.append(StructMemberConf("numField"      , "ushort" , 5, 2));
    s.members_conf.append(StructMemberConf("realNumField"  , "ushort" , 7, 2));
    s.members_conf.append(StructMemberConf("numLoader"     , "uchar"  , 9, 1));
    s.members_conf.append(StructMemberConf("dateOfLoad"    , "datetime" , 10, 3));
    s.members_conf.append(StructMemberConf("driver"        , "ushort"   , 13, 2));
    s.members_conf.append(StructMemberConf("pointOfAnal"   , "uchar"    , 15, 2));

    s.members_conf.append(StructMemberConf("bruttoWeight"  , "ushort"   , 0 + 16, 2));
    s.members_conf.append(StructMemberConf("dateOfBrutto"  , "datetime" , 2 + 16, 2));
    s.members_conf.append(StructMemberConf("taraWeight"    , "ushort"   , 5 + 16, 2));
    s.members_conf.append(StructMemberConf("normTaraWeight", "ushort"   , 7 + 16, 2));
    s.members_conf.append(StructMemberConf("dateOfTara"    , "datetime" , 9 + 16, 2));

    s.members_conf.append(StructMemberConf("impurity"      , "ushort"   , 0 + 32, 2));
    s.members_conf.append(StructMemberConf("shugarContent" , "ushort"   , 2 + 32, 2));
    s.members_conf.append(StructMemberConf("greenWeight"   , "ushort"   , 4 + 32, 2));
    s.members_conf.append(StructMemberConf("bum"           , "uchar"    , 6 + 32, 1));
    s.members_conf.append(StructMemberConf("bumFact"       , "uchar"    , 7 + 32, 1));
    s.members_conf.append(StructMemberConf("kagat"         , "uchar"    , 8 + 32, 1));
    s.members_conf.append(StructMemberConf("dateOfUnload"  , "datetime" , 9 + 32, 3));

    return s;
}

QByteArray MainSequence::card_code;

void MainSequence::onAppearOnWeight()
{
    qDebug() << "something appeared on weight!!!!";
    //tags["tablo"]->func("value", Q_ARG(QVariant, "Hello"));
    //need turn on red and off green and yellow

    //tags["do3"]->func("writeMethod", Q_ARG(QVariant, true));
    tags["reader1"]->func("doOn");
    on_weight = true;

    while(on_weight) {
        MifareCard card = tags["reader1"]->func("activateIdleA").value<MifareCard>();

        if ( !card.active() ) {
            qDebug() << "card not active!!!";

            sleepnb(1000); continue;
        }

        qDebug()<< "card active!";

        if ( !card.autorize(card_code, 128) ) {
            qDebug() << "fail to autorize!!!!";

            sleepnb(1000); continue;
        }

        QVariantMap map = card.readStruct(bill_conf());
        if ( map.isEmpty() ) {
            qDebug() << "readed bill is empty!!!";

            sleepnb(1000); continue;
        }
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
