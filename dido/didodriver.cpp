
#include "didodriver.h"

bool DidoDriver::registered = DidoDriver::registerInFact();

QVariant DidoDriver::getDi(IoDeviceWrapper::Pointer::Type* io, int num)
{
    QByteArray data = io->read(1);

    //qDebug () << "getDI: "<<data[0];

    return data[0] & ( 0x01 << num );
}

void DidoDriver::setDo(IoDeviceWrapper::Pointer::Type* io, int num, bool b)
{
    QByteArray data = io->read(1);
    data[0] = data[0] >> 4;

    if (b) { data[0] = data[0] | (1<<num);    }
    else   { data[0] = data[0] & (~(1<<num)); }

    io->write(data);
}
