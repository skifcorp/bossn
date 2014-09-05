#include "mettlertoledo310.h"
#include <string.h>
#include <QByteArray>


//bool MettlerToledo310::registered = MettlerToledo310::registerInFact();
BossnFactoryRegistrator<MettlerToledo310> MettlerToledo310::registraror("MettlerToledo310");

void MettlerToledo310::readWeight(QVariant & ret, uint & err)
{
    QByteArray req = weightRequestFrame();
    io_device()->write(req);

    const uchar frame_size = 8;
    while ( io_device()->bytesAvailable() < frame_size ) {
        yield();
    }

    QByteArray answ = io_device()->read(frame_size);

    ret = static_cast<int> (parseWeightFrameAnswer(answ, err));
}

void MettlerToledo310::zero( uint &)
{

}

QByteArray MettlerToledo310::weightRequestFrame() const
{
    QByteArray ba(1, 'P');

    return ba;
}

float MettlerToledo310::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
    /*QByteArray ret(6, 0);

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
         err = PorterFrameCorrupted;//throw WeightFrameExceptionCorrupted();
         return NAN;
    }
    err = 0;
    return fret;
    */
    const uchar frame_size = 8;
    if (ba.size() != frame_size) {
        err = PorterFrameCorrupted; return 0.0f;
    }
    bool ok = false;
    float fret = ba.mid(2,5).toFloat(&ok);
    if (!ok) {
        //throw WeightFrameExceptionCorrupted();
        err = PorterFrameCorrupted; return 0.0f;
    }
    err = 0;
    //qDebug() << "weight: " <<fret;
    return fret;

}


