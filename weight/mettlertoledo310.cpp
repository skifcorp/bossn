#include "mettlertoledo310.h"
#include <string.h>
#include <QByteArray>


bool MettlerToledo310::registered = MettlerToledo310::registerInFact();


void MettlerToledo310::readWeight(IoDeviceWrapper * io, float & ret, uint & err)
{
    QByteArray req = weightRequestFrame();
    io->write(req);

    const uchar frame_size = 8;
    while ( io->bytesAvailable() < frame_size ) {
        yield();
    }

    QByteArray answ = io->read(frame_size);

    ret = parseWeightFrameAnswer(answ, err);
}

void MettlerToledo310::zero(IoDeviceWrapper *, uint &)
{

}

QByteArray MettlerToledo310::weightRequestFrame() const
{
    QByteArray ba(1, 'P');

    return ba;
}

float MettlerToledo310::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
    QByteArray ret(6, 0);

    QByteArray::Iterator iter = ret.begin();
    if (ba[1] == 'I' || ba[1] == ' ' ) { *iter = '+';  }
    else if (ba[1] == 'i' || ba[1] == '-') { *iter = '-';}

    ++iter;

    for (int i = 2; i<7; ++i) {
        *iter = ba[i]; ++iter;
    }

    bool ok = false;
    float fret = ret.toFloat(&ok);
    if (!ok)  {
         err = WeightFrameCorrupted;//throw WeightFrameExceptionCorrupted();
         return NAN;
    }

    return fret;
}


