#include "mifarecard.h"
#include "func.h"

#include <QDate>
#include <QDateTime>
#include <QDataStream>
#include <QBitArray>


MifareCard::MifareCard(Tag::WeakPointer r, const ActivateCardISO14443A& ac):reader(r), activate_card(ac)
{

}

bool MifareCard::autorize(const QByteArray & key, int block)
{
    //HostCodedKey coded_key =  reader->getHostCodedKey(key);
    //qDebug () << "debugging autorize...";
    //printByteArray(key);
    QVariant key_var(key);

    HostCodedKey coded_key =  reader.data()->func("getHostCodedKey", Q_ARG(const QVariant&, key_var)).value<HostCodedKey>();

    if (!coded_key.valid()) {
        qWarning()<<"host coded key not valid!";
        return false;
    }

    AuthKey auth_key;
    auth_key.keys    = coded_key.coded;
    auth_key.snd     = activate_card.uid;
    auth_key.keyType = 0x60;
    auth_key.block  = block;

    //bool status = reader->doAuth(auth_key);
    QVariant auth_key_var = QVariant::fromValue<AuthKey>(auth_key);

    return reader.data()->func("doAuth", Q_ARG(QVariant, auth_key_var)).toBool();
}
/*
MifareRead MifareCard::readBlock(int num)
{
    //return reader->readBlock(num);
    QVariant num_var(num);

    //printByteArray();

    MifareRead r = reader.data()->func("readBlock", Q_ARG(const QVariant&, num_var)).value<MifareRead>();

    qDebug() << "read_block->block_num: "<<num;

    printByteArray(r.data);

    return r;
}*/

QVariant MifareCard::readMember(const StructMemberConf& mc, const QByteArray& arr) const
{
    if ( mc.offset + mc.length >= static_cast<uint>(arr.length()) ) {
        qWarning() << "ReadMember: Member struct with name: "<<mc.memberName<<" have offset: "<<mc.offset
                   <<" and length: "<<mc.length<<" which is bigger than raw data array size: "<<arr.length();
        return QVariant();
    }

    auto iter = StructMemberConf::typesFactoryForRead.find(mc.typeName);

    if (iter == StructMemberConf::typesFactoryForRead.end()) {
        qWarning() << "type: "<<mc.typeName<<" in member: "<<mc.memberName<<" not found!";
        return QVariant();
    }

    //qDebug() << "reading member: "<<mc.memberName;

    QByteArray tmp = arr.mid(mc.offset, mc.length);
    //printByteArray(tmp);

    return (*iter)(arr.mid(mc.offset, mc.length));
}

bool MifareCard::writeMember(const StructMemberConf &mc, const QVariant& val, QByteArray & arr) const
{
    if ( mc.offset + mc.length >= static_cast<uint>(arr.length()) ) {
        qWarning() << "WriteMember: struct with name: "<<mc.memberName<<" have offset: "<<mc.offset
                   <<" and length: "<<mc.length<<" which is bigger than raw data array size: "<<arr.length();
        return false;
    }

    auto iter = StructMemberConf::typesFactoryForWrite.find(mc.typeName);

    if (iter == StructMemberConf::typesFactoryForWrite.end()) {
        qWarning() << "WriteMember: type: "<<mc.typeName<<" in member: "<<mc.memberName<<" not found!";
        return false;
    }

    //qDebug() << "replacing "<<mc.length<<" bytes with "<<(*iter)(val).size()<<" bytes .. type: " << mc.typeName;



    QByteArray tmp = (*iter)(val);
    //qDebug() << "write member: "<<mc.memberName<< " val: "<<val;
    //printByteArray(tmp);
    arr.replace(mc.offset, mc.length, (*iter)(val));

    return true;
}

bool MifareCard::writeStruct(const StructConf &conf, const QVariantMap &s)
{
    QByteArray arr(conf.size(), 0);


    for (StructConf::MembersConf::const_iterator iter = conf.members_conf.begin(); iter != conf.members_conf.end(); ++iter) {
        writeMember(*iter, s[iter->memberName], arr);
        //qDebug() << "sizeof: " << arr.size();
    }

    uint offset_in_data = 0;
    for (int i = 0; i<conf.blocks.count(); ++i) {
        QByteArray block = arr.mid(offset_in_data, conf.blocks[i].blockSize);

        QVariant ret = reader.data()->func("writeBlock", Q_ARG(const QVariant&, conf.blocks[i].blockNum ), Q_ARG(const QVariant&, QVariant(block)));
        if ( !ret.toBool() ) {
            qWarning() << "MifareCard::writeStruct: cant write struct!!"; return false;
        }

        offset_in_data += conf.blocks[i].blockSize;
    }

    return true;
}

QVariantMap MifareCard::readStruct(const StructConf &conf)
{
    QVariantMap ret;
    QByteArray arr;
    for (int i = 0; i<conf.blocks.count(); ++i) {
        //qDebug () << "readStruct->before readBlock!";

        MifareRead mr = reader.data()->func("readBlock", Q_ARG(const QVariant&, QVariant(conf.blocks[i].blockNum))).value<MifareRead>();

        //qDebug() << "readStruct: block_num: " << conf.blocks[i].blockNum;

        //printByteArray(mr.data);

        if (!mr.result) {
            qWarning() << "readBlock seems failed!";

            return QVariantMap();
        }

        arr += mr.data;
    }

    for (int i = 0; i<conf.members_conf.count(); ++i) {
        QVariant val = readMember(conf.members_conf[i], arr);

        //qDebug() << "member: "<<conf.members_conf[i].memberName<<" val: "<<val;

        if (!val.isValid()) {return QVariantMap();}

        ret.insert( conf.members_conf[i].memberName,  val);
    }

    return ret;
}
