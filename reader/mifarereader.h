#ifndef MIFAREREADER_H
#define MIFAREREADER_H

#include <QByteArray>

#include "porterdriver.h"

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



class MifareReader : public PorterDriver
{
    Q_OBJECT
public:

    ~MifareReader();

    static PorterDriver * create(const QVariantMap& conf )
    {
        return new MifareReader(conf);
    }

    Q_INVOKABLE QVariant doOn();
    Q_INVOKABLE QVariant doOff();
    Q_INVOKABLE QVariant doSound( const QVariant& );
    Q_INVOKABLE QVariant activateIdleA();
    Q_INVOKABLE QVariant auth();
protected:
    MifareReader(const QVariantMap& );
private:
    static BossnFactoryRegistrator<MifareReader>  registator;

    uchar address;
    static uchar frame_ident;
};
/*
class Card
{
public:
    Card(MifareReader& r):reader(r){}
    ~Card() {}

    void readSector();
private:
};*/

#endif

