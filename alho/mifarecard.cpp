#include "mifarecard.h"
#include "func.h"

#include <QDate>
#include <QDateTime>
#include <QDataStream>
#include <QBitArray>



MifareCard::MifareCard(Tag::WeakPointer r, const ActivateCardISO14443A& ac,
                       const ReaderTagMethods& rs):reader(r), activate_card(ac), reader_settings(rs)
{

}

void MifareCard::autorize(const QByteArray & key, int block) throw (MifareCardException, MifareCardAuthException)
{
    //HostCodedKey coded_key =  reader->getHostCodedKey(key);
    //qDebug () << "debugging autorize...";
    //printByteArray(key);
    QVariant key_var(key);

    HostCodedKey coded_key =  reader.data()->func(reader_settings.host_coded_key,
                                                    Q_ARG(const QVariant&, key_var)).value<HostCodedKey>();

    if (!coded_key.valid()) {
        //qWarning()<<"host coded key not valid!";
        throw MifareCardException("host coded key not valid!");
    }

    AuthKey auth_key;
    auth_key.keys    = coded_key.coded;
    auth_key.snd     = activate_card.uid;
    auth_key.keyType = 0x60;
    auth_key.block  = block;

    QVariant auth_key_var = QVariant::fromValue<AuthKey>(auth_key);

    if ( !reader.data()->func(reader_settings.do_auth, Q_ARG(QVariant, auth_key_var)).toBool() ) {
        throw MifareCardAuthException("auth failed");
    }
}

QVariant MifareCard::readMember(const StructMemberConf& mc, const QByteArray& arr) const throw (MifareCardException)
{
    if ( mc.offset + mc.length >= static_cast<uint>(arr.length()) ) {
        throw MifareCardException( "ReadMember: Member struct with name: " + mc.memberName + " have offset: " +
                                   QString::number(mc.offset) + " and length: " + QString::number(mc.length) +
                                   " which is bigger than raw data array size: " + QString::number(arr.length()) );

    }

    auto iter = StructMemberConf::typesFactoryForRead.find(mc.typeName);

    if (iter == StructMemberConf::typesFactoryForRead.end()) {
        throw MifareCardException( "type: " + mc.typeName + " in member: " + mc.memberName + " not found!" );

    }


    return (*iter)(arr.mid(mc.offset, mc.length));
}

void MifareCard::writeMember(const StructMemberConf &mc, const QVariant& val, QByteArray & arr) const throw (MifareCardException)
{
    if ( mc.offset + mc.length >= static_cast<uint>(arr.length()) ) {
        throw MifareCardException( "WriteMember: struct with name: " + mc.memberName + " have offset: " +
                                    QString::number(mc.offset) + " and length: " + QString::number(mc.length) +
                                    " which is bigger than raw data array size: " + QString::number(arr.length()) );
    }

    auto iter = StructMemberConf::typesFactoryForWrite.find(mc.typeName);

    if (iter == StructMemberConf::typesFactoryForWrite.end()) {
        throw MifareCardException( "WriteMember: type: " + mc.typeName + " in member: " + mc.memberName + " not found!");
    }

    arr.replace(mc.offset, mc.length, (*iter)(val));
}

void MifareCard::writeStruct(const StructConf &conf, const QVariantMap &s) throw (MifareCardException)
{
    QByteArray arr(conf.size(), 0);
    try {
        for (StructConf::MembersConf::const_iterator iter = conf.members_conf.begin(); iter != conf.members_conf.end(); ++iter) {
            writeMember(*iter, s[iter->memberName], arr);
        }
    }
    catch (MifareCardException& ex)  {
        throw ex;
    }


    uint offset_in_data = 0;
    for (int i = 0; i<conf.blocks.count(); ++i) {
        QByteArray block = arr.mid(offset_in_data, conf.blocks[i].blockSize);

        QVariant ret = reader.data()->func(reader_settings.write_block, Q_ARG(const QVariant&, conf.blocks[i].blockNum ), Q_ARG(const QVariant&, QVariant(block)));
        if ( !ret.toBool() ) {
            throw MifareCardException("MifareCard::writeStruct: cant write struct!!");
        }

        offset_in_data += conf.blocks[i].blockSize;
    }
}

QVariantMap MifareCard::readStruct(const StructConf &conf) throw (MifareCardException)
{   
    QVariantMap ret;
    QByteArray arr;
    for (int i = 0; i<conf.blocks.count(); ++i) {
        MifareRead mr = reader.data()->func(reader_settings.read_block, Q_ARG(const QVariant&, QVariant(conf.blocks[i].blockNum))).value<MifareRead>();

        if (!mr.result) {
             throw MifareCardException("readBlock seems failed!");
        }

        arr += mr.data;
    }

    try {
        for (int i = 0; i<conf.members_conf.count(); ++i) {
            QVariant val = readMember(conf.members_conf[i], arr);
            ret.insert( conf.members_conf[i].memberName,  val);
        }

        return ret;
    }
    catch (MifareCardException & ex) {
        throw ex;
    }
}
