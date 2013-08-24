#include "elevatorpromservis.h"
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <typeinfo>


#include "func.h"

BossnFactoryRegistrator<EPS> EPS::registrator("EPS");

void EPS::readWeight(QVariant & ret, uint & err)
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

void EPS::zero(uint &)
{

}

QByteArray EPS::weightRequestFrame() const
{
    QByteArray ba;
    ba += "$";

    return ba;
}

int EPS::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
    const uchar frame_size = 16;
    if (ba.size() != frame_size) {
        err = PorterFrameCorrupted; return -1;
    }

    QByteArray ret = ba.mid(4,7);

    bool ok = false;
    int iret = ret.toDouble(&ok);

    if (!ok) {
        //throw WeightFrameExceptionCorrupted();
        err = PorterFrameCorrupted; return -1;
    }
    err = 0;
    return iret;
}



