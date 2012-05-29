#ifndef WINHANDLEDEVICE_H
#define WINHANDLEDEVICE_H

#include "iodevicewrapper.h"
#include "ISMMDLL.h"
#include <QFile>

class IsmmDevice : public IoDeviceWrapper
{
public:
    IsmmDevice ():handle(INVALID_HANDLE_VALUE) {}
    ~IsmmDevice ()
    {
        if(handle != INVALID_HANDLE_VALUE) ISMM_Close(handle);
    }

    virtual void setSettings(const QMap<QString, QVariant>& ) {}
    virtual QString deviceName() const { return QString();}
    virtual void clear() {}

    virtual bool open(QIODevice::OpenModeFlag )
    {
        handle = ISMM_Open();

        return handle != INVALID_HANDLE_VALUE;
    }

    qint64 virtual write ( const QByteArray& data )
    {
        //qDebug () << "ismm_device: data: "<< data[0] << " to_write: " <<static_cast<ushort> (~(data[0]>>4));

        bool ret = DIO_SetOutput( handle, static_cast<WORD> ( ~(data[0]>>4)) );

        return ret ? 1 : 0;
    }

    qint64 virtual bytesAvailable () const
    {
        return  handle != INVALID_HANDLE_VALUE ? 1 : 0;
    }
    QByteArray virtual read ( qint64 )
    {        
        return readAll();
    }
    QByteArray virtual readAll ()
    {
        WORD dis = 0;
        if (!DIO_GetInput(handle, &dis) ) return QByteArray();

        WORD dos = 0;
        if (!DIO_GetOutput(handle, &dos) ) return QByteArray();

        uchar ret = static_cast<uchar> (~(((dos & 0x000F)<<4) | ( dis & 0x000F)));

        return QByteArray(1, ret);
    }

    QByteArray virtual peek ( qint64 maxSize )
    {
        return readAll();
    }

    static IsmmDevice * create()
    {
        return new IsmmDevice();
    }
private:
    QFile fake_device;
    virtual QIODevice * internalGetDevice()
    {
        return &fake_device;
    }

    virtual const QIODevice * internalGetDevice() const
    {
        return &fake_device;
    }

    HANDLE handle;

    static BossnFactoryRegistrator<IsmmDevice> registrator;
};

#endif // WINHANDLEDEVICE_H
