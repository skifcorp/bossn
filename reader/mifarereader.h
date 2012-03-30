#ifndef MIFAREREADER_H
#define MIFAREREADER_H

#include <QByteArray>

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

#endif

