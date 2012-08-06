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

    const uchar frame_size = 6;

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
    ba += ";MSV?;";

    return ba;
}

int Hbm2110::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
/*    if (parseAddress(ba) != address) {
        //throw WeightFrameExceptionBadAddress();
        err = PorterFrameBadAddress; return -1;
    }*/

/*    QByteArray ret = ba.left(8);

    bool ok = false;
    float fret = ret.toInt(&ok);

    if (!ok) {
        //throw WeightFrameExceptionCorrupted();
        err = PorterFrameCorrupted; return -1;
    }

    //qDebug () << "getWeight: " << fret;
    err = 0;
    return fret;*/

    //printByteArray( ba );

    const uchar frame_size = 6;
    if (ba.size() != frame_size) {
        err = PorterFrameCorrupted; return -1;
    }

    int ret = static_cast<int>( (static_cast<uchar>(ba[0]) << 16)  | (static_cast<uchar>(ba[1]) << 8) | static_cast<uchar>(ba[2]));
    //qDebug() << "ret: "<< QString::number(ret, 16);
    return ret;
}



