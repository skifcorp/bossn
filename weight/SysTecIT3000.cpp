#include "SysTecIT3000.h"
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <typeinfo>



BossnFactoryRegistrator<SysTecIT3000> SysTecIT3000::registrator("SysTecIT3000");

void SysTecIT3000::readWeight(QVariant & ret, uint & err)
{
    QByteArray req = weightRequestFrame();

    io_device()->write(req);

    const uchar frame_size = 62;

    while ( io_device()->bytesAvailable() < frame_size ) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    ret = parseWeightFrameAnswer(answ, err);
}

void SysTecIT3000::zero(uint &)
{

}

QByteArray SysTecIT3000::weightRequestFrame() const
{
    QByteArray ba;
    ba += "<RN";
    ba += QByteArray::number(address);
    ba += ">";

    return ba;
}

int SysTecIT3000::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
/*    if (parseAddress(ba) != address) {
        err = PorterFrameBadAddress; return -1;
    } */

    const uchar frame_size = 62;
    if (ba.size() != frame_size) {
        err = PorterFrameCorrupted; return -1;
    }

    QByteArray ret = ba.mid(23, 8);

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



