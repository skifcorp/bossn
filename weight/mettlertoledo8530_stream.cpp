#include "mettlertoledo8530_stream.h"
#include <string.h>
#include <QByteArray>


//bool MettlerToledo8530::registered = MettlerToledo8530::registerInFact();

BossnFactoryRegistrator<MettlerToledo8530_stream> MettlerToledo8530_stream::registrator("MettlerToledo8530_stream");

void MettlerToledo8530_stream::readWeight(QVariant & ret, uint & err)
{
   /* qDebug() << "readWeight!";

    QByteArray req = weightRequestFrame();
    io_device()->write(req);
   */
    const uchar frame_size = 32;
    while ( io_device()->bytesAvailable() < frame_size ) {
        yield();
    }

    //qDebug () << "GOT SOMETHING!";

    QByteArray answ = io_device()->readAll();

    //qDebug () << answ;

    ret = static_cast<int> (parseWeightFrameAnswer(answ, err));

    //qDebug( ) << ret;
}

void MettlerToledo8530_stream::zero(uint &)
{
    QByteArray req = zeroRequestFrame(0);
    io_device()->write(req);
}

QByteArray MettlerToledo8530_stream::weightRequestFrame() const
{
    QByteArray cmd;
    /*cmd+='2';
    cmd+=QByteArray::number(address);
    cmd+='U';
    cmd+='B';
    cmd+=QByteArray::fromHex("0D");*/

    cmd += QByteArray::fromHex("02");
    cmd += QByteArray::number(address);
    cmd+='U';
    cmd+='B';
    cmd+=QByteArray::fromHex("0D");
    return cmd;
}

float MettlerToledo8530_stream::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
    /*const uchar frame_size = 13;
    if (ba.size() != frame_size) {
        err = PorterFrameCorrupted; return 0.0f;
    }

    if ( cmd.left(4) != ba.left(4) || ba.right(1)!=QByteArray::fromHex("0D") ) {
        //throw WeightFrameExceptionCorrupted();
        err = PorterFrameCorrupted; return 0.0f;
    }
    */
    int endbyte = ba.indexOf(QByteArray::fromHex("0D"),0);
   // qDebug() << "index of endbyte " << endbyte;
    float fret;

    if (endbyte != -1) {
        bool ok = false;
        fret = ba.mid(endbyte-11,5).toFloat(&ok);
        if (!ok) {
            //throw WeightFrameExceptionCorrupted();
            err = PorterFrameCorrupted; return 0.0f;
        }
    }
    else{
        fret = 0.0f;
    }
    err = 0;
    //qDebug() << "weight: " <<fret;
    return fret;
}

QByteArray MettlerToledo8530_stream::zeroRequestFrame(uchar) const
{
    return QByteArray::fromHex("02")+QByteArray::number(address)+"DK'@"+QByteArray::fromHex("0D");
}
void  MettlerToledo8530_stream::parseZeroFrameAnswer(const QByteArray&, uint & ) const
{

}

