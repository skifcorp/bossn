#include "esitPWI.h"
#include <QByteArray>

#include "coroutine.h"

bool EsitPWI::registered = EsitPWI::registerInFact();

void EsitPWI::readWeight(WeightDevice * io, float & ret, uint & err)
{
    QByteArray req = weightRequestFrame(err);
    if (err) return;

    io->write(req);

    const uchar frame_size = 8;
    while ( io->bytesAvailable() < frame_size ) {
        Coroutine::yield();
    }

    QByteArray answ = io->read(frame_size);

    ret = parseWeightFrameAnswer(answ, err);
}

void EsitPWI::zero(WeightDevice *, uint &)
{

}

QByteArray EsitPWI::weightRequestFrameM1() const
{
    return QByteArray(); //???
}
QByteArray EsitPWI::weightRequestFrameM3() const
{
    QByteArray a(2,0);
    a[0] = 0xFF;
    a[1] = address;
    return a;
}

float EsitPWI::parseWeightFrameAnswerM1(const QByteArray& ba, uint & err) const
{
    const uchar frame_size = 8;

    //if (ba.size() < frame_size) throw WeightFrameExceptionTooSmall();

    QByteArray ret(9,0);
    QByteArray::Iterator iter = ret.begin();

    for (int i = 0; i<frame_size; ++i) {
        if (ba[i] == 0x2B || ba[i] == 0x2D) {
            if (i+7 >= frame_size || ba[7] != 0x0D)  {
                //throw WeightFrameExceptionCorrupted();
                err = WeightFrameCorrupted; return NAN;
            }
            *iter = ba[i];
            for (int j = 1; j<7; ++j) {
                if (ba[i+j]>=0xB0) {
                    *iter = ba[i+j] - 0x80; ++iter;
                    *iter = '.'; ++iter;
                }
                else if ( ba[i+j]>=0x30 && ba[i+j]<=0x39 ) {
                    *iter = ba[i+j];
                }
                else if ( ba[i+j]!=0x20) {
                    //throw WeightFrameExceptionCorrupted();
                    err =  WeightFrameCorrupted; return NAN;
                }
            }
            break;
        }
    }

    bool ok = false; auto fret = ret.toFloat(&ok);
    if (!ok) {
        //throw WeightFrameExceptionCorrupted();
        err = WeightFrameCorrupted; return NAN;
    }
    return fret;
}

float EsitPWI::parseWeightFrameAnswerM3(const QByteArray& ba, uint & err) const
{
    //if (ba.size() < 8) throw WeightFrameExceptionTooSmall();

    if (ba[0] != 0x2B || ba[0] != 0x2D || ba[ba.size() - 1] != 0x0D){
        err = WeightFrameCorrupted; return NAN;
    }

    QByteArray ret(9,0);
    QByteArray::Iterator iter = ret.begin();

    if ( ba[0]==0x2D ) {
        *iter = '-'; ++iter;
    }

    for (int i=1; i<7; ++i) {
      if (ba[i]>=0xB0) {
        *iter = ba[i]-0x80; ++iter;
        *iter = '.'       ; ++iter;
      }
      else if ((ba[i]>=0x30) && (ba[i]<=0x39)) {
        *iter = ba[i]; ++iter;
      }
    }

    bool ok = false; auto fret = ret.toFloat(&ok);
    if (!ok) {
        //throw WeightFrameExceptionCorrupted();
        err = WeightFrameCorrupted; return NAN;
    }
    return fret;
}

