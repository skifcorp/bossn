
#include "didofile.h"


BossnFactoryRegistrator<DidoFile> DidoFile::registrator("DidoFile");

#include <iostream>

using std::cout;

QVariant DidoFile::getDi()
{
    QByteArray arr = io_device()->readAll();
    return QVariant( static_cast<uchar>(arr[0]) );
}

QVariant DidoFile::setDo(const QVariant& val)
{
    uchar b = static_cast<uchar>(val.toUInt());

    //qDebug () << "writing_do_byte: "<<b;

    io_device()->write(QByteArray(1, b));
    return QVariant(true);
}

QVariant DidoFile::getDiBit(const QVariant& full_byte, const QVariant& num)
{
    return (full_byte.toUInt()) & (0x01<<num.toUInt()) && true;
}

QVariant DidoFile::setDoBit(const QVariant& full_byte, const QVariant& num, const QVariant& val)
{
    uchar data = static_cast<uchar>(full_byte.toUInt());
    bool b = val.toBool();

    //qDebug () << "writing_do_bit: "<<full_byte.toUInt();

    if (b) {
        return data | (1<<(num.toUInt() + 4));
    }
    else {
        return data & (~(1<<(num.toUInt() + 4)));
    }
}




