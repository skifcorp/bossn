
#include "didofile.h"


BossnFactoryRegistrator<DidoFile> DidoFile::registrator("DidoFile");

#include <iostream>

using std::cout;

QVariant DidoFile::getDi()
{
    QByteArray arr = io_device()->readAll();
    /*for (int i = 0; i<8; ++i) {
        cout <<  (( arr[0] >> i ) & 1) ;
    }*/
    //cout << "\n";
    return QVariant( static_cast<uchar>(arr[0]) );
}

void DidoFile::setDo(const QVariant& val)
{
/*    uchar data = readAll(io);
    data = data >> 4;
    if (b) { data = data | (1<<num.toUInt());    }
    else   { data = data & (~(1<<num.toUInt())); } */

/*    WDTPARAM cParam;
    DWORD nReturn;

    //cParam.data_b = data;
    cParam.data_b = static_cast<uchar>(val.toUInt());
    BOOL ret = io_device()->DeviceIoControl(IOCTL_SYS_DIO_WRITE, &cParam, sizeof(WDTPARAM), 0, 0, &nReturn, NULL);
    if (!ret) {
        qWarning("Cant write dido!!!!");
    }*/
    uchar b = static_cast<uchar>(val.toUInt());
    io_device()->write(QByteArray(1, b));
}

QVariant DidoFile::getDiBit(const QVariant& full_byte, const QVariant& num)
{
    return (full_byte.toUInt()) & (0x01<<num.toUInt()) && true;
}

QVariant DidoFile::setDoBit(const QVariant& full_byte, const QVariant& num, const QVariant& val)
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




