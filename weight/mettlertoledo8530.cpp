#include "mettlertoledo8530.h"
#include <string.h>
#include <QByteArray>


//bool MettlerToledo8530::registered = MettlerToledo8530::registerInFact();

BossnFactoryRegistrator<MettlerToledo8530> MettlerToledo8530::registrator("MettlerToledo8530");

void MettlerToledo8530::readWeight(float & ret, uint & err)
{
    QByteArray req = weightRequestFrame();
    io_device()->write(req);

    const uchar frame_size = 13;
    while ( io_device()->bytesAvailable() < frame_size ) {
        yield();
    }

    QByteArray answ = io_device()->read(frame_size);

    ret = parseWeightFrameAnswer(answ, err);
}

void MettlerToledo8530::zero(uint &)
{
    QByteArray req = zeroRequestFrame(0);
    io_device()->write(req);
}

QByteArray MettlerToledo8530::weightRequestFrame() const
{
    QByteArray cmd;
    cmd+='2';
    cmd+=QByteArray::number(address);
    cmd+='U';
    cmd+='B';
    cmd+=QByteArray::fromHex("0D");
    return cmd;
}

float MettlerToledo8530::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{

    if (ba[0] != '2' || ba[2] != 'U' || ba[3] != 'B' || ba.right(1) != QByteArray::fromHex("0D")) {
        //throw WeightFrameExceptionCorrupted();
        err = WeightFrameCorrupted; return NAN;
    }

    if ( static_cast<uchar>(QString( ba[1] ).toUInt()) != address ) {
        //throw WeightFrameExceptionBadAddress();
        err = WeightFrameBadAddress; return NAN;
    }

    bool ok = false;
    bool fret = ba.mid(4,8).toFloat(&ok);
    if (!ok) {
        //throw WeightFrameExceptionCorrupted();
        err = WeightFrameCorrupted; return NAN;
    }
    return fret;
}

QByteArray MettlerToledo8530::zeroRequestFrame(uchar) const
{
    return QByteArray::fromHex("02")+QByteArray::number(address)+"DK'@"+QByteArray::fromHex("0D");
}
void  MettlerToledo8530::parseZeroFrameAnswer(const QByteArray&, uint & ) const
{

}

