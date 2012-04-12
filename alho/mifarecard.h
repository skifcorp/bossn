#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QVariant>
#include <QMap>

#include <functional>

using std::function;

#include "mifarereader.h"
#include "tags.h"


struct StructMemberConf
{
    QString memberName;
    QString typeName;
    uint offset;
    uint length;
    StructMemberConf(const QString& mn, const QString& tn, uint offs, uint len):memberName(mn), typeName(tn), offset(offs), length(len) {}

    typedef QMap<QString, function<QVariant (const QByteArray& )> > TypesFactory;
    static TypesFactory typesFactory;

    static bool registerTypes();
};

struct StructConf
{
    typedef QList<StructMemberConf> MembersConf;
    MembersConf  members_conf;
    QList<uint> blocks;
    bool empty () const {return members_conf.empty();}
};

class MifareCard
{
public:
    MifareCard(Tag::WeakPointer r , const ActivateCardISO14443A& );

    ~MifareCard(){}

    bool active() const {return activate_card.active();}

    bool autorize( const QByteArray& , int block );
    MifareRead readBlock(int num);



    QVariantMap readStruct(const StructConf& conf);


    QByteArray uid() const {return activate_card.uid;}  
private:
    Tag::WeakPointer reader;
    ActivateCardISO14443A activate_card;

    QVariant readMember (const StructMemberConf& , const QByteArray& ) const;


    //friend class MifareReader;
};



#endif // CARD_H
