#include "hbm2108.h"
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <typeinfo>



BossnFactoryRegistrator<Hbm2108> Hbm2108::registrator("Hbm2108");

void Hbm2108::readWeight(QVariant & ret, uint & err)
{
    QByteArray req = weightRequestFrame();

    io_device()->write(req);

    const uchar frame_size = 16;

    while ( io_device()->bytesAvailable() < frame_size ) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    ret = parseWeightFrameAnswer(answ, err);
}

void Hbm2108::zero(uint &)
{

}

QByteArray Hbm2108::weightRequestFrame() const
{
    QByteArray ba;
    ba += "S";
    ba += QByteArray::number(address);
    ba += ";COF5;MSV?;";

    return ba;
}

int Hbm2108::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
/*    if (parseAddress(ba) != address) {
        err = PorterFrameBadAddress; return -1;
    } */

    const uchar frame_size = 16;
    if (ba.size() != frame_size) {
        err = PorterFrameCorrupted; return -1;
    }

    QByteArray ret = ba.mid(4, 7);

    bool ok = false;
    float fret = ret.toFloat(&ok);

    if (!ok) {
        //throw WeightFrameExceptionCorrupted();
        err = PorterFrameCorrupted; return -1;
    }

    qDebug () << "getWeight: " << fret;
    err = 0;
    return fret;
}



