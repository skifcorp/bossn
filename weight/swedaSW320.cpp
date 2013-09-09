#include "swedasw320.h"
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <typeinfo>
#include <boost/crc.hpp>

#include "func.h"

BossnFactoryRegistrator<SwedaSW320> SwedaSW320::registrator("SwedaSW320");

void SwedaSW320::readWeight(QVariant & ret, uint & err)
{
    QByteArray req = weightRequestFrame();

    io_device()->write(req);

    const uchar frame_size = 9;

    while ( io_device()->bytesAvailable() < frame_size ) {
        yield();
    }

    QByteArray answ = io_device()->readAll();

    ret = parseWeightFrameAnswer(answ, err);
}

void SwedaSW320::zero(uint &)
{

}

QByteArray SwedaSW320::weightRequestFrame() const
{
    QByteArray a;
    a.push_back(address);
    a.push_back('\3');
    a.push_back('\0');
    a.push_back('\0');
    a.push_back('\0');
    a.push_back('\2');

    boost::crc_optimal<16, 0x8005, 0xFFFF, 0, true, true> crc;
    crc.process_bytes(a.data(), 6);
    unsigned short chksum = crc.checksum();

    auto low  = (reinterpret_cast<char *>(&chksum))[0];
    auto high = (reinterpret_cast<char *>(&chksum))[1];
 
    a.push_back(low);
    a.push_back(high);
	
	return a;
}

int SwedaSW320::parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
{
    const uchar frame_size = 9;
    if (ba.size() != frame_size) {
        err = PorterFrameCorrupted; return -1;
    }

    QByteArray ret;
    ret.push_back( ba[6] );
    ret.push_back( ba[5] );
    ret.push_back( ba[4] );
    ret.push_back( ba[3] );

    err = 0;

    return *reinterpret_cast<int *>(ret.data());
    //bool ok = false;
    //int iret = ret.toDouble(&ok);

    //if (!ok) {
     //   //throw WeightFrameExceptionCorrupted();
    //    err = PorterFrameCorrupted; return -1;
    //}
    //err = 0;
    //return iret;
}


