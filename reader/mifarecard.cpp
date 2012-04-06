#include "mifarecard.h"

const int MifareCard::id = qRegisterMetaType<MifareCard>();

MifareCard::MifareCard(MifareReader* r, const ActivateCardISO14443A& ac):reader(r), activate_card(ac)
{

}

bool MifareCard::autorize(const QByteArray & key, int sector)
{
    HostCodedKey coded_key =  reader->getHostCodedKey(key);

    if (!coded_key.valid()) return false;

    AuthKey auth_key;
    auth_key.keys    = coded_key.coded;
    auth_key.snd     = activate_card.uid;
    auth_key.keyType = 0x60;
    auth_key.sector  = sector;

    char status = reader->doAuth(auth_key);

    return status != 0;
}

MifareRead MifareCard::readBlock(int num)
{
    return reader->readBlock(num);
}

QVariantMap MifareCard::readStruct(const StructConf &conf)
{
    QVariantMap ret;
    for (int i = 0; i<conf.sectors.count(); ++i) {

    }

    return ret;
}
