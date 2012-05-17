#ifndef CARDSTRUCTS_H
#define CARDSTRUCTS_H

#include <QString>
#include <QMap>
#include <QByteArray>
#include <QVariant>
#include <QDateTime>

#include <functional>

using std::function;

struct StructMemberConf
{
    QString memberName;
    QString typeName;
    uint offset;
    uint length;
    StructMemberConf(const QString& mn, const QString& tn, uint offs, uint len)
        :memberName(mn), typeName(tn), offset(offs), length(len) {}

    StructMemberConf():offset(0), length(0){}

    typedef QMap<QString, function<QVariant   (const QByteArray& )> > TypesFactoryForRead;
    typedef QMap<QString, function<QByteArray (const QVariant&   )> > TypesFactoryForWrite;

    static TypesFactoryForRead  typesFactoryForRead;
    static TypesFactoryForWrite typesFactoryForWrite;

    static bool registerTypes();
    //uint offset() const {return offset + baseOffsetInData;}
};

struct BlockConf
{
    BlockConf() :blockNum(0) {}
    BlockConf(uint bn,  uint bs):blockNum(bn),blockSize(bs) {}
    uint blockNum;
    uint blockSize;
};

struct StructConf
{
    typedef QList<StructMemberConf> MembersConf;    
    MembersConf  members_conf;

    typedef QList<BlockConf> BlocksConf;
    BlocksConf blocks;

    bool empty () const {return members_conf.empty();}
    uint size() const;
};


inline const StructConf & bill_conf(const QVariantMap& opts)
{
    static StructConf s;

    if (!s.empty()) return s;

    uint data_block = opts["data_block"].toUInt();

    /*s.blocks.append(0 + data_block);
    s.blocks.append(1 + data_block);
    s.blocks.append(2 + data_block); */

    s.blocks.append(BlockConf(data_block + 0, 16));
    s.blocks.append(BlockConf(data_block + 1, 16));
    s.blocks.append(BlockConf(data_block + 2, 16));

    s.members_conf.append(StructMemberConf("billNumber"    , "uint"     ,     0 + 0, 3));
    s.members_conf.append(StructMemberConf("flags"         , "boolarr"  ,     3 + 0, 2));
    s.members_conf.append(StructMemberConf("numField"      , "ushort"   ,     5 + 0, 2));
    s.members_conf.append(StructMemberConf("realNumField"  , "ushort"   ,     7 + 0, 2));
    s.members_conf.append(StructMemberConf("numLoader"     , "uchar"    ,     9 + 0, 1));
    s.members_conf.append(StructMemberConf("dateOfLoad"    , "datetimeshit", 10 + 0, 3));
    s.members_conf.append(StructMemberConf("driver"        , "ushort"   ,    13 + 0, 2));
    s.members_conf.append(StructMemberConf("pointOfAnal"   , "uchar"    ,    15 + 0, 1));

    s.members_conf.append(StructMemberConf("bruttoWeight"  , "ushort"       , 0 + 16, 2));
    s.members_conf.append(StructMemberConf("dateOfBrutto"  , "datetimeshit" , 2 + 16, 3));
    s.members_conf.append(StructMemberConf("taraWeight"    , "ushort"       , 5 + 16, 2));
    s.members_conf.append(StructMemberConf("normTaraWeight", "ushort"       , 7 + 16, 2));
    s.members_conf.append(StructMemberConf("dateOfTara"    , "datetimeshit" , 9 + 16, 3));

    s.members_conf.append(StructMemberConf("impurity"      , "ushort"       , 0 + 32, 2));
    s.members_conf.append(StructMemberConf("shugarContent" , "ushort"       , 2 + 32, 2));
    s.members_conf.append(StructMemberConf("greenWeight"   , "ushort"       , 4 + 32, 2));
    s.members_conf.append(StructMemberConf("bum"           , "uchar"        , 6 + 32, 1));
    s.members_conf.append(StructMemberConf("bumFact"       , "uchar"        , 7 + 32, 1));
    s.members_conf.append(StructMemberConf("kagat"         , "uchar"        , 8 + 32, 1));
    s.members_conf.append(StructMemberConf("dateOfUnload"  , "datetimeshit" , 9 + 32, 3));

    return s;
}

QDateTime timeShitToDateTime(ulong timeInMinutes);
ulong dateTimeToTimeShit(const QDateTime& dt);

#endif // CARDSTRUCTS_H
