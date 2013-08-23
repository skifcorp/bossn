#include "mifarereaderemulator.h"
#include "mifarereader.h"

#include "func.h"
#include "settingstool.h"

using std::cout;


#include "mifarecarddata.h"
#include "cardstructs.h"

BossnFactoryRegistrator<MifareReaderEmulator> MifareReaderEmulator::registator("MifareReaderEmulator");

MifareReaderEmulator::MifareReaderEmulator(const QVariantMap& s)
{
    qDebug () << "MifareReaderEmulator CREATED!";
    card_class_name = get_setting<QString>("card_class_name", s);
}

MifareReaderEmulator::~MifareReaderEmulator()
{

}


QVariant MifareReaderEmulator::doOn()
{
    return QVariant(true);
}

QVariant MifareReaderEmulator::doOff()
{
    return QVariant(true);
}


QVariant MifareReaderEmulator::doSound(const QVariant& )
{
    return QVariant(true);
}


QVariant MifareReaderEmulator::activateIdleA()
{
    ActivateCardISO14443A ac;
    ac.uid += "000000";

    return QVariant::fromValue<ActivateCardISO14443A>(ac);
}


QVariant MifareReaderEmulator::getHostCodedKey(const QVariant& )
{
    HostCodedKey coded_key;
    coded_key.ack   = 0;
    coded_key.coded = QByteArray(6, 0);

    return QVariant::fromValue<HostCodedKey>(coded_key);
}


MifareReaderEmulator::current_block_offsets MifareReaderEmulator::getCurrentBlockOffsets(int num) const
{
    current_block_offsets o;
    const BlocksConf& bc = CardStructs::blocks_conf();

    for ( int i = 0; i<num; ++i )  {
        o.start_offset += bc[i].blockSize;
    }

    o.finish_offset = o.start_offset + bc[num].blockSize;

    return o;
}

QVariant MifareReaderEmulator::doAuth(const QVariant& )//const AuthKey & auth)
{
    return QVariant(true);
}

MifareCardData MifareReaderEmulator::defaultDataForBlock(int num) const
{
    checkBlockNum(num);

    const StructConf & sc =  CardStructs::bill_conf(card_class_name);

    MifareCardData one_block;

    current_block_offsets offsets = getCurrentBlockOffsets(num);

    for ( auto iter = sc.members_conf.begin(); iter != sc.members_conf.end(); ++iter) {
        if ( (*iter).offset >= offsets.start_offset && (*iter).offset < offsets.finish_offset  ) {
            //qDebug()<< "num: " << num << " foundOffset: " << (*iter).offset << " value: "<<iter->defaultValue().toString();
            one_block.insert( iter->memberName, iter->defaultValue() );
        }
    }

    return one_block;
}

QByteArray MifareReaderEmulator::cardDataToByteArray( const MifareCardData& data, int num ) const
{
    const StructConf& sc = CardStructs::bill_conf(card_class_name);
    const BlocksConf& bc = CardStructs::blocks_conf();

    current_block_offsets offsets = getCurrentBlockOffsets(num);

    QByteArray ret( bc[num].blockSize, 0 );
    for (MifareCardData::const_iterator iter = data.begin(); iter != data.end(); ++iter ) {
        const StructMemberConf& smc = sc.findByMember( iter.key() );
        QByteArray arr = StructMemberConf::functionForWrite( smc.typeName )( *iter );
        ret.replace( smc.offset - offsets.start_offset, smc.length, arr );
    }
    return ret;
}

void MifareReaderEmulator::checkBlockNum(int num) const
{
    if ( num >= CardStructs::blocks_conf().count() ) {
        qWarning() << " MifareCardEmulator: you passed block_num " << num << " but blocks_count is: " <<
                      CardStructs::blocks_conf().count();
        qFatal("Exiting...");
    }

    if ( num >=  CardStructs::bill_conf(card_class_name).members_conf.count() ) {
        qWarning() << " MifareCardEmulator: you passed block_num " << num << " but blocks_count is: " <<
                      CardStructs::bill_conf(card_class_name).members_conf.count();
        qFatal("Exiting...");
    }
}

int MifareReaderEmulator::blockIndexByNum(uint num) const
{
    const BlocksConf& bc = CardStructs::blocks_conf() ;
    int counter = 0;
    for ( auto iter = bc.begin(); iter != bc.end(); ++iter , ++counter) {
        if (iter->blockNum == num) return counter;
    }

    qWarning() << "MifareReaderEmulator: cant find blockConf with blockNum == " << num;
    qFatal("Exiting!"); return 0;
}

QVariant MifareReaderEmulator::readBlock(const QVariant& n)
{
    int num = blockIndexByNum( n.toInt() );

    checkBlockNum( num );

    MifareRead ret;
    ret.result = 0;
    ret.result = true;

    try  {
        MifareCardData bill = defaultDataForBlock( num );

        if ( num == 0 ) { //must refactor this
            bill.setMemberValue("driver", 390);
            bill.setMemberValue("billNumber", 1);
            bill.setMemberValue("numField", 105);
        }
        else if ( num == 1) {
            bill.setMemberValue("bruttoWeight", 10000);
            bill.setMemberValue("dateOfBrutto", QDateTime(QDate(2013, 01, 29), QTime(12, 20, 0)));
        }
        else if ( num == 2 ) {
            bill.setMemberValue("bumFact", 1);
            bill.setMemberValue("kagat", 1);
            bill.setMemberValue("material", 21);
        }

        ret.data = cardDataToByteArray( bill, num );
    }
    catch ( FuncNotFoundException& e ) {
        qWarning() << "func_not_found exception! name: " << e.funcName();
        qFatal("exiting!");
    }



    return QVariant::fromValue<MifareRead>(ret);
}

QVariant MifareReaderEmulator::writeBlock(const QVariant& , const QVariant& )
{
    return QVariant(true);
}
