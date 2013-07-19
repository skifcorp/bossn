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
#include "mifarecarddata.h"
#include "mainsequencesettings.h"

class MifareCardException
{
public:
    MifareCardException(const QString& user_msg, const QString& adm_msg):
        user_message_(user_msg), admin_message_(adm_msg){}

    virtual ~MifareCardException() {}

    QString userMessage() const {return user_message_;}
    QString adminMessage() const {return admin_message_;}

private:
    QString user_message_;
    QString admin_message_;

};

class MifareCardReadException : public MifareCardException
{
public:
    MifareCardReadException(const QString& user_msg, const QString& adm_msg):MifareCardException(user_msg, adm_msg){}

    virtual ~MifareCardReadException() {}
};

class MifareCardWriteException : public MifareCardException
{
public:
    MifareCardWriteException(const QString& user_msg, const QString& adm_msg):MifareCardException(user_msg, adm_msg){}

    virtual ~MifareCardWriteException() {}
};


class MifareCardAuthException : public MifareCardException
{
public:
    MifareCardAuthException(const QString& user_msg, const QString& adm_msg) : MifareCardException(user_msg, adm_msg) {}
    ~MifareCardAuthException () {}
private:
};

class MifareCardSector
{
public:
    MifareCardSector( const  ActivateCardISO14443A&, ReaderTagMethods&, const QByteArray& cd, uint db );

    ~MifareCardSector(){}

    bool active() const {return activate_card.active();}

    void autorize(  );
    //QByteArray readByteArray(const StructConf& conf) throw (MifareCardException);
    QByteArray readByteArray(const BlocksConf& ) ;
    QByteArray readBlock(const BlockConf& ) ;

    MifareCardData readStruct(const QByteArray&, const StructConf& conf) ;


    void writeStruct(const StructConf& conf, const MifareCardData& s,
                     const BlocksConf& );
    void writeByteArray(const QByteArray&, const BlocksConf& );
    void writeBlock( const QByteArray& , const BlockConf& );

    QString toString( const StructConf& conf, const MifareCardData& s )const ;
    QString toBigString(const StructConf& conf, const MifareCardData& s) const ;

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

    ReaderTagMethods& readerTagMethods() const
    {
        return reader_settings;
    }

private:
    //Tag::WeakPointer reader;
    ActivateCardISO14443A activate_card;
    //int reader_num = -1;

    QVariant readMember (const StructMemberConf& , const QByteArray& ) const ;
    void     writeMember(const StructMemberConf& , const QVariant&, QByteArray& ) const;

    ReaderTagMethods& reader_settings;
    QByteArray card_key;
    uint data_block;    
};



#endif // CARD_H
