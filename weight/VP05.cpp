#include "VP05.h"
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <typeinfo>



BossnFactoryRegistrator<VP05> VP05::registrator("VP05");

void VP05::readWeight(QVariant & ret, uint & err)
{
    QByteArray req = weightRequestFrame();

    io_device()->write(req);

    const uchar frame_size = 14;

    while ( io_device()->bytesAvailable() < frame_size ) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    ret = parseWeightFrameAnswer(answ, err);
}

void VP05::zero(uint &)
{

}

QByteArray VP05::weightRequestFrame() const
{
    QByteArray ba;
    ba.push_back(0x02);
    ba.push_back(0x41);
    ba.push_back(0x42);
    ba.push_back(0x30);
    ba.push_back(0x33);
    ba.push_back(0x03);

    return ba;
}

int VP05::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{

    const uchar frame_size = 14;
    if (ba.size() != frame_size) {
        err = PorterFrameCorrupted; return -1;
    }

    QByteArray ret = ba.mid(3, 8);

    bool ok = false;
    float fret = ret.toFloat(&ok);

    if (!ok) {
        err = PorterFrameCorrupted; return -1;
    }

    //qDebug () << "getWeight: " << fret;
    err = 0;
    return fret;
}



