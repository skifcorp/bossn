#include <QDebug>

#include "cardstructs.h"

QMap<QString, StructConf> CardStructs::bill_classes;
BlocksConf CardStructs::default_blocks_conf;

bool CardStructs::initialized = CardStructs::initialize();



const  BlocksConf& CardStructs::blocks_conf()
{
/*    if ( !initialized )
        initialize();*/

    return default_blocks_conf;
}

const StructConf& CardStructs::bill_conf(const QString& class_name)
{
/*    if (!initialized)
        initialize();*/

    QMap<QString, StructConf>::const_iterator iter = bill_classes.find(class_name);
    if (iter == bill_classes.end()) {
        qWarning() << "cant find bill_conf with class_name: " <<class_name;
        qFatal("exit");
    }

    return *iter;
}

bool CardStructs::initialize()
{
    default_blocks_conf.append(BlockConf(128 + 0, 16));
    default_blocks_conf.append(BlockConf(128 + 1, 16));
    default_blocks_conf.append(BlockConf(128 + 2, 16));

    StructConf beet_acceptance;

    //beet_acceptance.blocks.append(BlockConf(128 + 0, 16));
    //beet_acceptance.blocks.append(BlockConf(128 + 1, 16));
    //beet_acceptance.blocks.append(BlockConf(128 + 3, 16));

    beet_acceptance.members_conf.append(StructMemberConf("billNumber"    , "uint"     ,     0 + 0, 3));
    beet_acceptance.members_conf.append(StructMemberConf("flags"         , "boolarr"  ,     3 + 0, 2));
    beet_acceptance.members_conf.append(StructMemberConf("numField"      , "ushort"   ,     5 + 0, 2));
    beet_acceptance.members_conf.append(StructMemberConf("realNumField"  , "ushort"   ,     7 + 0, 2));
    beet_acceptance.members_conf.append(StructMemberConf("numLoader"     , "uchar"    ,     9 + 0, 1));
    beet_acceptance.members_conf.append(StructMemberConf("dateOfLoad"    , "datetimeshit", 10 + 0, 3));
    beet_acceptance.members_conf.append(StructMemberConf("driver"        , "ushort"   ,    13 + 0, 2));
    beet_acceptance.members_conf.append(StructMemberConf("pointOfAnal"   , "uchar"    ,    15 + 0, 1));

    beet_acceptance.members_conf.append(StructMemberConf("bruttoWeight"  , "ushort"       , 0 + 16, 2));
    beet_acceptance.members_conf.append(StructMemberConf("dateOfBrutto"  , "datetimeshit" , 2 + 16, 3));
    beet_acceptance.members_conf.append(StructMemberConf("taraWeight"    , "ushort"       , 5 + 16, 2));
    beet_acceptance.members_conf.append(StructMemberConf("normTaraWeight", "ushort"       , 7 + 16, 2));
    beet_acceptance.members_conf.append(StructMemberConf("dateOfTara"    , "datetimeshit" , 9 + 16, 3));

    beet_acceptance.members_conf.append(StructMemberConf("impurity"      , "ushort"       , 0 + 32, 2));
    beet_acceptance.members_conf.append(StructMemberConf("shugarContent" , "ushort"       , 2 + 32, 2));
    beet_acceptance.members_conf.append(StructMemberConf("greenWeight"   , "ushort"       , 4 + 32, 2));
    beet_acceptance.members_conf.append(StructMemberConf("bum"           , "uchar"        , 6 + 32, 1));
    beet_acceptance.members_conf.append(StructMemberConf("bumFact"       , "uchar"        , 7 + 32, 1));
    beet_acceptance.members_conf.append(StructMemberConf("kagat"         , "uchar"        , 8 + 32, 1));
    beet_acceptance.members_conf.append(StructMemberConf("dateOfUnload"  , "datetimeshit" , 9 + 32, 3));
    beet_acceptance.members_conf.append(StructMemberConf("material"      , "uchar"        ,15 + 32, 1));

    bill_classes["BeetAcceptanceWeighter"] = beet_acceptance;


    StructConf grain_acceptance;

    grain_acceptance.members_conf.append(StructMemberConf("billNumber"    , "uint"     ,     0 + 0, 3));
    grain_acceptance.members_conf.append(StructMemberConf("flags"         , "boolarr"  ,     3 + 0, 2));
    grain_acceptance.members_conf.append(StructMemberConf("numField"      , "ushort"   ,     5 + 0, 2));
    grain_acceptance.members_conf.append(StructMemberConf("realNumField"  , "ushort"   ,     7 + 0, 2));
    grain_acceptance.members_conf.append(StructMemberConf("numLoader"     , "uchar"    ,     9 + 0, 1));
    grain_acceptance.members_conf.append(StructMemberConf("dateOfLoad"    , "datetimeshit", 10 + 0, 3));
    grain_acceptance.members_conf.append(StructMemberConf("driver"        , "ushort"   ,    13 + 0, 2));
    grain_acceptance.members_conf.append(StructMemberConf("pointOfAnal"   , "uchar"    ,    15 + 0, 1));

    grain_acceptance.members_conf.append(StructMemberConf("bruttoWeight"  , "ushort"       , 0 + 16, 2));
    grain_acceptance.members_conf.append(StructMemberConf("dateOfBrutto"  , "datetimeshit" , 2 + 16, 3));
    grain_acceptance.members_conf.append(StructMemberConf("taraWeight"    , "ushort"       , 5 + 16, 2));
    grain_acceptance.members_conf.append(StructMemberConf("normTaraWeight", "ushort"       , 7 + 16, 2));
    grain_acceptance.members_conf.append(StructMemberConf("dateOfTara"    , "datetimeshit" , 9 + 16, 3));
    grain_acceptance.members_conf.append(StructMemberConf("culture"       , "uchar"        ,12 + 16, 1));
    grain_acceptance.members_conf.append(StructMemberConf("sort"          , "uchar"        ,13 + 16, 1));
    grain_acceptance.members_conf.append(StructMemberConf("repr"          , "uchar"        ,14 + 16, 1));


    grain_acceptance.members_conf.append(StructMemberConf("impurity"      , "ushort"       , 0 + 32, 2));
    grain_acceptance.members_conf.append(StructMemberConf("shugarContent" , "ushort"       , 2 + 32, 2));
    grain_acceptance.members_conf.append(StructMemberConf("greenWeight"   , "ushort"       , 4 + 32, 2));
    grain_acceptance.members_conf.append(StructMemberConf("bum"           , "uchar"        , 6 + 32, 1));
    grain_acceptance.members_conf.append(StructMemberConf("reserv1"       , "uchar"        , 7 + 32, 1));
    grain_acceptance.members_conf.append(StructMemberConf("kagat"         , "uchar"        , 8 + 32, 1));
    grain_acceptance.members_conf.append(StructMemberConf("bumFact"       , "uchar"        , 9 + 32, 1));
    grain_acceptance.members_conf.append(StructMemberConf("dateOfUnload"  , "datetimeshit" ,10 + 32, 3));
    grain_acceptance.members_conf.append(StructMemberConf("reserv2"       , "uchar"        ,13 + 32, 1));
    grain_acceptance.members_conf.append(StructMemberConf("prefixcar"     , "uchar"        ,14 + 32, 1));
    grain_acceptance.members_conf.append(StructMemberConf("material"      , "uchar"        ,15 + 32, 1));

    bill_classes["GrainAcceptanceWeighter"] = grain_acceptance;

    StructConf grain_shipment = grain_acceptance;

    bill_classes["GrainShipmentWeighter"] = grain_shipment;

    return true;
}
