#include "hbm2110.h"
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <typeinfo>


//bool Hbm2110::registered = Hbm2110::registerInFact();
BossnFactoryRegistrator<Hbm2110> Hbm2110::registrator("Hbm2110");

void Hbm2110::readWeight(IoDeviceWrapper * io, QVariant & ret, uint & err)
{
    QByteArray req = weightRequestFrame();

    io->write(req);

    const uchar frame_size = 17;
    //qDebug()<<"bytes: "<<io->bytesAvailable();
    while ( io->bytesAvailable() < frame_size ) {        
        yield();
    }

    QByteArray answ = io->read(frame_size);

    ret = parseWeightFrameAnswer(answ, err);
}

void Hbm2110::zero(IoDeviceWrapper *, uint &)
{

}

QByteArray Hbm2110::weightRequestFrame() const
{
    QByteArray ba;
    ba += "S";
    ba += QByteArray::number(address);
    ba += ";MSV?;";

    return ba;
}

float Hbm2110::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
    if (parseAddress(ba) != address) {
        //throw WeightFrameExceptionBadAddress();
        err = WeightFrameBadAddress; return NAN;
    }

    QByteArray ret = ba.left(8);

    bool ok = false;
    float fret = ret.toFloat(&ok);

    if (!ok) {
        //throw WeightFrameExceptionCorrupted();
        err = WeightFrameCorrupted; return NAN;
    }

    //qDebug () << "getWeight: " << fret;

    return fret;
}



