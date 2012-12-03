#include "mifarereaderlist.h"
#include "settingstool.h"
#include "factory.h"

BossnFactoryRegistrator<MifareReaderList> MifareReaderList::registator("MifareReaderList");

MifareReaderList::MifareReaderList(const QVariantMap& otp): activeReader(-1)
{
    QVariantList l = get_setting<QVariantList> ("addresses", otp);
    for (int i=0; i<l.count();++i){
        readers.append(PorterDriver::create("MifareReader",l[i].toMap()));

    }

}

MifareReaderList::~MifareReaderList()
{

}

QVariant MifareReaderList::doOn()
{
    //qDebug("start do on");
    //yield();
    //qDebug("start do on 2");
    for (int i=0; i<readers.count();++i){
        readers[i]->setIoDevice( io_device() );
        if (!readers[i].staticCast<MifareReader>()->doOn().toBool() ){
            qDebug ()<< "error do on" << i;
            return QVariant(false);
        }
    }
    // qDebug("end do on");
    return QVariant(true);
}
QVariant MifareReaderList::doOff()
{

    for (int i=0; i<readers.count();++i){
        if (!readers[i].staticCast<MifareReader>()->doOff().toBool() )
            return QVariant(false);
     }
    return QVariant(true);
}
QVariant MifareReaderList::doSound( const QVariant& imp)
{
    QVariant ret = readers[activeReader].staticCast<MifareReader>()-> doSound(imp);
    activeReader = -1;
    return ret;

}
QVariant MifareReaderList::activateIdleA()
{
    for (int i=0; i<readers.count();++i){
        ActivateCardISO14443A act = readers[i].staticCast<MifareReader>()->activateIdleA().value<ActivateCardISO14443A>();
        if (act.active()){
            activeReader = i;
            return QVariant::fromValue<ActivateCardISO14443A>(act)  ;
        }
    }

    return QVariant::fromValue<ActivateCardISO14443A>(ActivateCardISO14443A())  ;
}
QVariant MifareReaderList::getHostCodedKey(const QVariant& key)
{
    Q_ASSERT(activeReader > -1);
    return readers[activeReader].staticCast<MifareReader>()->getHostCodedKey(key);
}
QVariant MifareReaderList::doAuth( const QVariant& key)
{
    Q_ASSERT(activeReader > -1);
    QVariant ret = readers[activeReader].staticCast<MifareReader>()->doAuth(key);
    if (!ret.toBool())
        activeReader = -1;

    return ret;

}
QVariant MifareReaderList::readBlock( const QVariant& num)
{
    Q_ASSERT(activeReader > -1);
    return readers[activeReader].staticCast<MifareReader>()->readBlock(num);

}
QVariant MifareReaderList::writeBlock( const QVariant& num , const QVariant& data )
{
    Q_ASSERT(activeReader > -1);
    QVariant ret = readers[activeReader].staticCast<MifareReader>()-> writeBlock(num, data);

    return ret;
}
