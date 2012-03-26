
#include "didodriver.h"
#include "wdt.h"

bool DidoIt8718f::registered = DidoIt8718f::registerInFact();

uchar DidoIt8718f::readAll(IoDeviceWrapper::Pointer::Type* io)
{
    WDTPARAM cParam;
    DWORD nReturn;

    BOOL ret = io->DeviceIoControl(IOCTL_SYS_DIO_READ, &cParam, sizeof(WDTPARAM), &cParam, sizeof(WDTPARAM), &nReturn, NULL);

    if (!ret) {
        qWarning("Cant read dido!!!!"); return 0;
    }

    return cParam.timeout;
}

QVariant DidoIt8718f::getDi(IoDeviceWrapper::Pointer::Type* io, int num)
{
    return readAll(io) & ( 0x01 << num );
}

void DidoIt8718f::setDo(IoDeviceWrapper::Pointer::Type* io, int num, bool b)
{
    uchar data = readAll(io);
    data = data >> 4;
    if (b) { data = data | (1<<num);    }
    else   { data = data & (~(1<<num)); }

    WDTPARAM cParam;
    DWORD nReturn;

    cParam.data_b = data;
    BOOL ret = io->DeviceIoControl(IOCTL_SYS_DIO_WRITE, &cParam, sizeof(WDTPARAM), 0, 0, &nReturn, NULL);
    if (!ret) {
        qWarning("Cant write dido!!!!");
    }
}
