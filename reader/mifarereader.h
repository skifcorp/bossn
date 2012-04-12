#ifndef MIFAREREADER_H
#define MIFAREREADER_H

#include <QByteArray>


#include "porterdriver.h"
#include "sharedfromthis.h"

struct MifareRequestFrame
{
    static const int paramsBuffLen = 300;

    MifareRequestFrame();
    ~MifareRequestFrame();

    QByteArray packFrame();

    void print() const;
    int size() const;

    static const uchar startCondition = 0xFD;
    uchar           address;
    uchar           ident;
    uchar           cmdCode;
    ushort          crc16;
    QByteArray      params;
    static const uchar finishCondition = 0xFE;

    static QByteArray staffBytes(const QByteArray& );
};

struct MifareResponseFrame
{
    MifareResponseFrame();
    ~MifareResponseFrame();

    bool checkCrc() const;
    void print() const;
    int size() const;
    bool checkResponse(const MifareRequestFrame& );

    uchar   startCondition;
    uchar   address;
    uchar   ident;
    uchar   cmdCode;
    char     cmdStatus;
    QByteArray       params;
    ushort  crc16;
    uchar   finishCondition;

    bool unpackFrame(const QByteArray& );
    static QByteArray unstaffBytes(const QByteArray& );
};



struct ActivateCardISO14443A {
    char ack;
    QByteArray atq;
    uchar sak;
    QByteArray uid;
    ActivateCardISO14443A () : ack(0),sak(0) {}
    bool active() const {return !uid.isEmpty();}
    static int id;
};

Q_DECLARE_METATYPE(ActivateCardISO14443A)


struct HostCodedKey {
    char ack;
    QByteArray coded;
    HostCodedKey() : ack(0) {}

    bool valid() const {return !coded.isEmpty();}
    static int id;
};

Q_DECLARE_METATYPE(HostCodedKey)

struct AuthKey {
    unsigned char keyType;
    QByteArray snd;
    QByteArray keys;
    unsigned char block;

    AuthKey() : snd(0), block(0) {}
    static int id;
};

Q_DECLARE_METATYPE(AuthKey)

struct MifareRead
{
    MifareRead() : ack(0) {}
    uchar ack;
    QByteArray data;

    static int id;
};

Q_DECLARE_METATYPE(MifareRead)

class MifareReader : public PorterDriver//, public SharedFromThis<MifareReader>
{
    Q_OBJECT
public:


    MifareReader(const MifareReader& ) = delete;
    //MifareReader(){}
    ~MifareReader();

    static PorterDriver * create(const QVariantMap& conf )
    {
        return new MifareReader(conf);
    }

    Q_INVOKABLE QVariant doOn();
    Q_INVOKABLE QVariant doOff();
    Q_INVOKABLE QVariant doSound( const QVariant& );
    Q_INVOKABLE QVariant activateIdleA();   
    Q_INVOKABLE QVariant getHostCodedKey(const QVariant& );
    Q_INVOKABLE QVariant doAuth( const QVariant& );
    Q_INVOKABLE QVariant readBlock( const QVariant& );
    Q_INVOKABLE QVariant writeBlock( const QVariant&, const QByteArray&  );
protected:
    MifareReader(const QVariantMap& );
private:
    static BossnFactoryRegistrator<MifareReader>  registator;

    uchar address;
    static uchar frame_ident;


    friend class MifareCard;
    bool waitForAnswer();

    static QString errorMessage(uchar e);
};



#endif

