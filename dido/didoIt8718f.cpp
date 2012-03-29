
#include "didoIt8718f.h"
#include "wdt.h"


BossnFactoryRegistrator<DidoIt8718f> DidoIt8718f::registator("DidoIt8718f");

uchar DidoIt8718f::readAll()
{
    WDTPARAM cParam;
    DWORD nReturn;

    BOOL ret = io_device()->DeviceIoControl(IOCTL_SYS_DIO_READ, &cParam, sizeof(WDTPARAM), &cParam, sizeof(WDTPARAM), &nReturn, NULL);

    if (!ret) {
        qWarning("Cant read dido!!!!"); return 0;
    }

    return cParam.timeout;
}

//QVariant DidoIt8718f::getDi(IoDeviceWrapper::Pointer::Type* io, const QVariant& num)
QVariant DidoIt8718f::getDi()
{
    //qDebug() << "getDI!!!!";

    return readAll();// & ( 0x01 << num.toUInt() );
}

//void DidoIt8718f::setDo(IoDeviceWrapper::Pointer::Type* io, const QVariant& num, bool b)
void DidoIt8718f::setDo(const QVariant& val)
{
/*    uchar data = readAll(io);
    data = data >> 4;
    if (b) { data = data | (1<<num.toUInt());    }
    else   { data = data & (~(1<<num.toUInt())); } */

    WDTPARAM cParam;
    DWORD nReturn;

    //cParam.data_b = data;
    cParam.data_b = static_cast<uchar>(val.toUInt());
    BOOL ret = io_device()->DeviceIoControl(IOCTL_SYS_DIO_WRITE, &cParam, sizeof(WDTPARAM), 0, 0, &nReturn, NULL);
    if (!ret) {
        qWarning("Cant write dido!!!!");
    }
}

QVariant DidoIt8718f::getDiBit(const QVariant& full_byte, const QVariant& num)
{
    //qDebug() << "getDiBit!!!!!!!";

    return (full_byte.toUInt()) & (0x01<<num.toUInt()) && true;
}

QVariant DidoIt8718f::setDoBit(const QVariant& full_byte, const QVariant& num, const QVariant& val)
{
    uchar data = static_cast<uchar>(full_byte.toInt());
    bool b = val.toBool();

    data = data >> 4;
    if (b) {
        return data | (1<<num.toUInt());
    }
    else {
        return data & (~(1<<num.toUInt()));
    }
}



