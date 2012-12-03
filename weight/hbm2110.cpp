#include "Hbm2110.h"
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <typeinfo>


#include "func.h"

BossnFactoryRegistrator<Hbm2110> Hbm2110::registrator("Hbm2110");

void Hbm2110::readWeight(QVariant & ret, uint & err)
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

void Hbm2110::zero(uint &)
{

}

QByteArray Hbm2110::weightRequestFrame() const
{
    QByteArray ba;
    ba += "S";
    ba += QByteArray::number(address);
    ba += ";COF5;MSV?;";

    return ba;
}

int Hbm2110::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
    const uchar frame_size = 16;
    if (ba.size() != frame_size) {
        err = PorterFrameCorrupted; return -1;
    }

    QByteArray ret = ba.mid(4,7);
    bool ok = false;
    int iret = ret.toInt(&ok);
    //qDebug () << "getWeight: " << fret;

    if (!ok) {
        //throw WeightFrameExceptionCorrupted();
        err = PorterFrameCorrupted; return -1;
    }
    err = 0;
    return iret;
}



