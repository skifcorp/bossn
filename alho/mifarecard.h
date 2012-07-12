#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QVariant>
#include <QMap>

#include <functional>

using std::function;

#include "mifarereader.h"
#include "tags.h"
#include "cardstructs.h"
#include "readersettings.h"
#include "alhosequence.h"

class MifareCardException
{
public:
    MifareCardException(const QString& msg):message_(msg){}

    virtual ~MifareCardException() {}

    QString message() const {return message_;}

private:
    QString message_;

};

class MifareCardAuthException : public MifareCardException
{
public:
    MifareCardAuthException(const QString& msg) : MifareCardException(msg) {}
    ~MifareCardAuthException () {}
private:
};

class MifareCard
{
public:
    MifareCard(Tag::WeakPointer r , const ActivateCardISO14443A&, const ReaderTagMethods&, AlhoSequence& c );

    ~MifareCard(){}

    bool active() const {return activate_card.active();}

    void autorize( const QByteArray& , int block ) throw (MifareCardException, MifareCardAuthException);
    QVariantMap readStruct(const StructConf& conf) throw (MifareCardException);

    void writeStruct(const StructConf& conf, const QVariantMap& s) throw (MifareCardException);

    QByteArray uid() const {return activate_card.uid;}  
    static uchar passwordBlock(uchar block)
    {
        //const uint manufacturerBlocks = 2;
        const uint blocksIn1KSector  = 4;
        const uint blocksIn4KSector  = 16;
        const uint blocksIn1KSectors = blocksIn1KSector * 32;

        //if (block < manufacturerBlocks) return 0;

        if (block < blocksIn1KSectors) {
            return block / blocksIn1KSector * blocksIn1KSector + 3 ;
        }

        return blocksIn1KSectors + (block - blocksIn1KSectors) / blocksIn4KSector * blocksIn4KSector + 15;
    }

private:
    Tag::WeakPointer reader;
    ActivateCardISO14443A activate_card;

    QVariant readMember (const StructMemberConf& , const QByteArray& ) const throw (MifareCardException);
    void     writeMember(const StructMemberConf& , const QVariant&, QByteArray& ) const throw (MifareCardException);

    const ReaderTagMethods& reader_settings;
    AlhoSequence& caller;
};



#endif // CARD_H
