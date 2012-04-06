#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QVariant>


#include "mifarereader.h"

struct StructMemberConf
{
    QString memberName;
    QString typeName;
    uint offset;
    uint length;
    StructMemberConf(const QString& mn, const QString& tn, uint offs, uint len):memberName(mn), typeName(tn), offset(offs), length(len) {}
};

struct StructConf
{
    typedef QList<StructMemberConf> MembersConf;
    MembersConf  members_conf;
    QList<uint> sectors;
    bool empty () const {return members_conf.empty();}
};

class MifareCard
{
public:
    static const int id;
    MifareCard():reader(nullptr){}
    ~MifareCard(){}

    bool active() const {return activate_card.active();}

    bool autorize( const QByteArray& , int sector );
    MifareRead readBlock(int num);



    QVariantMap readStruct(const StructConf& conf);


protected:
    MifareCard(MifareReader* , const ActivateCardISO14443A& );
private:
    //MifareReader::WeakPointer reader;
    MifareReader * reader;
    ActivateCardISO14443A activate_card;

    friend class MifareReader;
};

Q_DECLARE_METATYPE(MifareCard)

#endif // CARD_H
