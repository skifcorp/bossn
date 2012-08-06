#include "mettlertoledo8530.h"
#include <string.h>
#include <QByteArray>


//bool MettlerToledo8530::registered = MettlerToledo8530::registerInFact();

BossnFactoryRegistrator<MettlerToledo8530> MettlerToledo8530::registrator("MettlerToledo8530");

void MettlerToledo8530::readWeight(QVariant & ret, uint & err)
{
    //qDebug() << "readWeight!";

    QByteArray req = weightRequestFrame();
    io_device()->write(req);

    const uchar frame_size = 13;
    while ( io_device()->bytesAvailable() < frame_size ) {
        yield();
    }

    //qDebug () << "GOT SOMETHING!";

    QByteArray answ = io_device()->readAll();



    ret = static_cast<int> (parseWeightFrameAnswer(answ, err, req));

    //qDebug( ) << ret;
}

void MettlerToledo8530::zero(uint &)
{
    QByteArray req = zeroRequestFrame(0);
    io_device()->write(req);
}

QByteArray MettlerToledo8530::weightRequestFrame() const
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

float MettlerToledo8530::parseWeightFrameAnswer(const QByteArray& ba, uint & err, const QByteArray& cmd) const
{
    const uchar frame_size = 13;
    if (ba.size() != frame_size) {
        err = PorterFrameCorrupted; return 0.0f;
    }

    if ( cmd.left(4) != ba.left(4) || ba.right(1)!=QByteArray::fromHex("0D") ) {
        //throw WeightFrameExceptionCorrupted();
        err = PorterFrameCorrupted; return 0.0f;
    }


    bool ok = false;
    float fret = ba.mid(4,8).toFloat(&ok);
    if (!ok) {
        //throw WeightFrameExceptionCorrupted();
        err = PorterFrameCorrupted; return 0.0f;
    }
    err = 0;
    //qDebug() << "weight: " <<fret;
    return fret;
}

QByteArray MettlerToledo8530::zeroRequestFrame(uchar) const
{
    return QByteArray::fromHex("02")+QByteArray::number(address)+"DK'@"+QByteArray::fromHex("0D");
}
void  MettlerToledo8530::parseZeroFrameAnswer(const QByteArray&, uint & ) const
{

}

