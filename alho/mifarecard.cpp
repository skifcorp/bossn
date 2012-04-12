#include "mifarecard.h"

#include <QDate>
#include <QDateTime>
#include <QDataStream>
#include <QBitArray>

StructMemberConf::TypesFactory StructMemberConf::typesFactory;

bool types_registered = StructMemberConf::registerTypes();


QDateTime convertTimeShit(unsigned long timeInMinutes)
{
    QDate d(2000, 01, 01);
    QDateTime dt(d);

    unsigned long temp=timeInMinutes/60/24;
    int ost=timeInMinutes-temp*60*24;

    dt=dt.addDays(temp);

    int hour=ost/60;
    int minute=ost-hour*60;

    QTime time(hour, minute);
    dt.setTime(time);
    return dt;
}

bool StructMemberConf::registerTypes()
{
    typesFactory.insert("uint", [](const QByteArray& arr){
                            QDataStream st(arr);
                            uint ret;
                            st >> ret;
                            return QVariant(ret);
                        });


    typesFactory.insert("boolarr", [](const QByteArray& arr){
                            QDataStream st(arr);
                            QBitArray ret;
                            st >> ret;
                            return QVariant(ret);
                        });

    typesFactory.insert("ushort", [](const QByteArray& arr){
                            QDataStream st(arr);
                            ushort ret;
                            st >> ret;
                            return QVariant(ret);
                        });

    typesFactory.insert("uchar", [](const QByteArray& arr){
                            return QVariant(static_cast<uchar>(arr[0]));
                        });


    typesFactory.insert("datetimeshit", [](const QByteArray& arr) {
                            QDataStream st(arr);
                            uint ret;
                            st >> ret;
                            return QVariant( convertTimeShit(ret) );
                        });


    return true;
}

MifareCard::MifareCard(Tag::WeakPointer r, const ActivateCardISO14443A& ac):reader(r), activate_card(ac)
{

}

bool MifareCard::autorize(const QByteArray & key, int block)
{
    //HostCodedKey coded_key =  reader->getHostCodedKey(key);
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

MifareRead MifareCard::readBlock(int num)
{
    //return reader->readBlock(num);
    QVariant num_var(num);
    return reader.data()->func("readBlock", Q_ARG(const QVariant&, num_var)).value<MifareRead>();
}

QVariant MifareCard::readMember(const StructMemberConf& mc, const QByteArray& arr) const
{
    if ( mc.offset + mc.length >= static_cast<uint>(arr.length()) ) {
        qWarning() << "Member struct with name: "<<mc.memberName<<" have offset: "<<mc.offset
                   <<" and length: "<<mc.length<<" which is bigger than raw data array size: "<<arr.length();
        return QVariant();
    }

    auto iter = StructMemberConf::typesFactory.find(mc.typeName);

    if (iter == StructMemberConf::typesFactory.end()) {
        qWarning() << "type: "<<mc.typeName<<" in member: "<<mc.memberName<<" not found!";
        return QVariant();
    }

    return (*iter)(arr.mid(mc.offset, mc.length));
}

QVariantMap MifareCard::readStruct(const StructConf &conf)
{
    QVariantMap ret;
    QByteArray arr;
    for (int i = 0; i<conf.blocks.count(); ++i) {
        MifareRead mr = reader.data()->func("readBlock", Q_ARG(const QVariant&, QVariant(conf.blocks[i]))).value<MifareRead>();
        if (mr.ack != 0) {return QVariantMap();}

        arr += mr.data;
    }

    for (int i = 0; i<conf.members_conf.count(); ++i) {
        QVariant val = readMember(conf.members_conf[i], arr);
        if (!val.isValid()) {return QVariantMap();}

        ret.insert( conf.members_conf[i].memberName,  val);
    }

    return ret;
}


