#include "mifarecard.h"
#include "func.h"

#include <QDate>
#include <QDateTime>
#include <QDataStream>
#include <QBitArray>



MifareCardBlock::MifareCardBlock(const ActivateCardISO14443A& ac,
                       ReaderTagMethods& rs, const QByteArray& cd, uint db):
                       activate_card(ac), reader_settings(rs), card_key(cd),
                       data_block(db)
{

}

void MifareCardBlock::autorize() throw (MifareCardException, MifareCardAuthException)
{
    QVariant key_var(card_key);

//    HostCodedKey coded_key =  reader.data()->func(reader_settings.host_coded_key, &caller,
//                                                    Q_ARG(const QVariant&, key_var)).value<HostCodedKey>();

    HostCodedKey coded_key = reader_settings.host_coded_key.func(Q_ARG(const QVariant&, key_var)).value<HostCodedKey>();

    if (!coded_key.valid()) {
        //qWarning()<<"host coded key not valid!";
        throw MifareCardException("host coded key not valid!");
    }

    AuthKey auth_key;
    auth_key.keys    = coded_key.coded;
    auth_key.snd     = activate_card.uid;
    auth_key.keyType = 0x60;
    auth_key.block  = data_block;

    QVariant auth_key_var = QVariant::fromValue<AuthKey>(auth_key);

/*    if ( !reader.data()->func(reader_settings.do_auth, &caller, Q_ARG(QVariant, auth_key_var)).toBool() ) {
        throw MifareCardAuthException("auth failed");
    }*/
    if ( !reader_settings.do_auth.func( Q_ARG(QVariant, auth_key_var)).toBool() ) {
        throw MifareCardAuthException("auth failed");
    }

}

QVariant MifareCardBlock::readMember(const StructMemberConf& mc, const QByteArray& arr) const throw (MifareCardException)
{
    if ( mc.offset + mc.length > static_cast<uint>(arr.length()) ) {
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

void MifareCardBlock::writeMember(const StructMemberConf &mc, const QVariant& val, QByteArray & arr) const throw (MifareCardException)
{
    if ( mc.offset + mc.length > static_cast<uint>(arr.length()) ) {
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

void MifareCardBlock::writeStruct(const StructConf &conf, const MifareCardData &s, const BlocksConf& bc) throw (MifareCardException)
{
    QByteArray arr(bc.memorySize(), 0);
    try {
        for (StructConf::MembersConf::const_iterator iter = conf.members_conf.begin(); iter != conf.members_conf.end(); ++iter) {
            writeMember(*iter, s[iter->memberName], arr);
        }
    }
    catch (MifareCardException& ex)  {
        throw ex;
    }


    uint offset_in_data = 0;
    for (int i = 0; i<bc.count(); ++i) {
        QByteArray block = arr.mid(offset_in_data, bc[i].blockSize);

        auto write_func = [&]  {
            return reader_settings.write_block.func(
                        Q_ARG(const QVariant&, bc[i].blockNum),
                        Q_ARG(const QVariant&, QVariant(block)) );
         };

         QVariant ret = write_func();

         if (!ret.toBool()) {
             for (int j = 0; j<10; ++j) {
                 qDebug () << "try rewrite...";
                 try {
                    reader_settings.do_off.func();
                    reader_settings.do_on.func();
                    reader_settings.activate_idle.func();
                    autorize();
                    ret = write_func();
                    if (ret.toBool())
                        break;
                 }
                 catch (...) {
                     qDebug()<<"try rewrite FAILED";
                 }
             }
         }


        if ( !ret.toBool() ) {
            throw MifareCardWriteException(
                        QObject::tr("MifareCard::writeStruct: cant write struct!!"));
        }

        offset_in_data += bc[i].blockSize;
    }
}

QString MifareCardBlock::toString(const StructConf &conf, const MifareCardData &s) const throw()
{
    QString ret;
    for (StructConf::MembersConf::const_iterator iter = conf.members_conf.begin(); iter != conf.members_conf.end(); ++iter) {
        if ( iter->typeName == "boolarr") {
            QBitArray bits = s[iter->memberName].toBitArray();
            for (int i = 0; i<bits.count(); ++i) {
                ret += bits.testBit(i) ? "1 " : "0 ";
            }
            ret += ", ";
        }
        else {
            ret += s[iter->memberName].toString() + ", ";
        }
    }

    return ret.left( ret.length() - 2 );
}


QString MifareCardBlock::toBigString(const StructConf &conf, const MifareCardData &s) const throw()
{
    QString ret;

    for (StructConf::MembersConf::const_iterator iter = conf.members_conf.begin(); iter != conf.members_conf.end(); ++iter) {
        QString row = iter->memberName.leftJustified(16) + "=" ;

        if ( iter->typeName == "boolarr") {
            QBitArray bits = s[iter->memberName].toBitArray();
            for (int i = 0; i<bits.count(); ++i) {
                row += bits.testBit(i) ? "1 " : "0 ";
            }
            row += "\n";
        }
        else {
            row += s[iter->memberName].toString() + "\n";
        }
        ret += row;
    }

    return ret;
}


QByteArray MifareCardBlock::readByteArray(const BlocksConf& conf) throw(MifareCardException)
{
    //MifareCardData ret;
    //ret.setUid(uid());

    QByteArray arr;
    for (int i = 0; i<conf.count(); ++i) {
        //MifareRead mr = reader.data()->func(reader_settings.read_block, &caller, Q_ARG(const QVariant&, QVariant(conf.blocks[i].blockNum))).value<MifareRead>();
        MifareRead mr;
        for (int j = 0; j<10; ++j) {
            mr = reader_settings.read_block.func( Q_ARG(const QVariant&, QVariant(conf[i].blockNum))).value<MifareRead>();
            if (mr.result) break;
        }

        if (!mr.result) {
             throw MifareCardReadException(QObject::tr("readBlock seems failed!"));
        }

        arr += mr.data;
    }
    return arr;
}

MifareCardData MifareCardBlock::readStruct(const QByteArray &arr, const StructConf& conf) throw (MifareCardException)
{   
    MifareCardData ret;
    ret.setUid(uid());

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
