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
    bool checkResponce(const MifareRequestFrame& );

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
  ActivateCardISO14443A () : ack(0),sak(0)
  {}
  bool active() const {return !uid.isEmpty();}

};



struct HostCodedKey {
  signed char ack;
  QByteArray coded;
  HostCodedKey() : ack(0) {}

  bool valid() const {return !coded.isEmpty();}
};


struct AuthKey {
  unsigned char keyType;
/*  unsigned char snd[4];
  unsigned char keys[12]; */
  QByteArray snd;
  QByteArray keys;
  unsigned char sector;

  AuthKey() : snd(0), sector(0) {}

};


struct MifareRead
{
    MifareRead() : ack(0) {}
    uchar ack;
    QByteArray data;
};

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

    HostCodedKey getHostCodedKey(const QByteArray& key);
    bool doAuth(const AuthKey& );
    MifareRead readBlock(int num);
protected:
    MifareReader(const QVariantMap& );
private:
    static BossnFactoryRegistrator<MifareReader>  registator;

    uchar address;
    static uchar frame_ident;


    friend class MifareCard;
    bool waitForAnswer();
};



#endif

