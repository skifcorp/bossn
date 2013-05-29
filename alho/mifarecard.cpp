#include "mifarecard.h"
#include "func.h"

#include <QDate>
#include <QDateTime>
#include <QDataStream>
#include <QBitArray>



MifareCardSector::MifareCardSector(const ActivateCardISO14443A& ac,
                       ReaderTagMethods& rs, const QByteArray& cd, uint db):
                       activate_card(ac), reader_settings(rs), card_key(cd),
                       data_block(db)
{

}

void MifareCardSector::autorize()
{
    QVariant key_var(card_key);

    HostCodedKey coded_key = reader_settings.host_coded_key.func(Q_ARG(const QVariant&, key_var)).value<HostCodedKey>();

    if (!coded_key.valid()) {
        throw MifareCardException(QObject::tr("host coded key not valid!"), "host coded key not valid!");
    }

    AuthKey auth_key;
    auth_key.keys    = coded_key.coded;
    auth_key.snd     = activate_card.uid;
    auth_key.keyType = 0x60;
    auth_key.block  = data_block;

    QVariant auth_key_var = QVariant::fromValue<AuthKey>(auth_key);

    if ( !reader_settings.do_auth.func( Q_ARG(QVariant, auth_key_var)).toBool() ) {
        throw MifareCardAuthException(QObject::tr("auth failed"), "auth failed");
    }

}

QVariant MifareCardSector::readMember(const StructMemberConf& mc, const QByteArray& arr) const
{
    if ( mc.offset + mc.length > static_cast<uint>(arr.length()) ) {
        const QString txt = "ReadMember: Member struct with name: " + mc.memberName + " have offset: " +
                QString::number(mc.offset) + " and length: " + QString::number(mc.length) +
                " which is bigger than raw data array size: " + QString::number(arr.length());
        throw MifareCardException( txt, txt );
    }

    try {
        return StructMemberConf::functionForRead(mc.typeName)(arr.mid(mc.offset, mc.length));
    }
    catch (FuncNotFoundException& e) {
        const QString txt = "type: " + mc.typeName + " in member: " + e.funcName() + " not found!";
        throw MifareCardException(  txt, txt );
    }
}

void MifareCardSector::writeMember(const StructMemberConf &mc, const QVariant& val, QByteArray & arr) const
{
    if ( mc.offset + mc.length > static_cast<uint>(arr.length()) ) {
        const QString txt = "WriteMember: struct with name: " + mc.memberName + " have offset: " +
                QString::number(mc.offset) + " and length: " + QString::number(mc.length) +
                " which is bigger than raw data array size: " + QString::number(arr.length());
        throw MifareCardException( txt, txt );
    }

    try {
        arr.replace(mc.offset, mc.length, StructMemberConf::functionForWrite( mc.typeName )(val));
    }
    catch(FuncNotFoundException & e) {
        const QString txt = "WriteMember: type: " + mc.typeName + " in member: " + e.funcName() + " not found!";
        throw MifareCardException( txt, txt );
    }
}

void MifareCardSector::writeByteArray( const QByteArray& arr, const BlocksConf& bc )
{
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
                    auto act = reader_settings.activate_idle.func().value<ActivateCardISO14443A>();
                    if ( !act.active() )
                        continue;
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
                        QObject::tr("MifareCard::writeStruct: cant write struct!!"),
                        "MifareCard::writeStruct: cant write struct!!" );
        }

        offset_in_data += bc[i].blockSize;
    }

}

void MifareCardSector::writeStruct(const StructConf &conf, const MifareCardData &s, const BlocksConf& bc)
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


    writeByteArray( arr, bc );
}

QString MifareCardSector::toString(const StructConf &conf, const MifareCardData &s) const
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


QString MifareCardSector::toBigString(const StructConf &conf, const MifareCardData &s) const
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


QByteArray MifareCardSector::readByteArray(const BlocksConf& conf)
{
    //MifareCardData ret;
    //ret.setUid(uid());

    QByteArray arr;
    for (int i = 0; i<conf.count(); ++i) {
        MifareRead mr;
        for (int j = 0; j<10; ++j) {
            mr = reader_settings.read_block.func( Q_ARG(const QVariant&, QVariant(conf[i].blockNum))).value<MifareRead>();
            if (mr.result) break;
        }

        if (!mr.result) {
             throw MifareCardReadException(QObject::tr("readBlock seems failed!"),
                                           "readBlock seems failed!");
        }

        arr += mr.data;
    }
    return arr;
}

MifareCardData MifareCardSector::readStruct(const QByteArray &arr, const StructConf& conf)
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
