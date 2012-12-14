#ifndef IODEVICEEMULATOR_H
#define IODEVICEEMULATOR_H




#include "iodevicewrapper.h"
#include "settingstool.h"

class IoDeviceEmulator : public IoDeviceWrapper
{
public:
    virtual void setSettings(const QMap<QString, QVariant>& ) {}
    virtual QString deviceName() const {return "IoDeviceEmulator";}

    virtual void clear() {}


    virtual bool open(QIODevice::OpenModeFlag )
    {
        return is_opened = true;
    }

    qint64 virtual write ( const QByteArray& data )
    {
        return data.length();
    }

    qint64 virtual bytesAvailable () const
    {
        return 0;
    }
    QByteArray virtual read ( qint64  )
    {
        return QByteArray();
    }
    QByteArray virtual readAll ()
    {
        return QByteArray();
    }

    QByteArray virtual peek ( qint64  )
    {
        return QByteArray();
    }

    static IoDeviceWrapper * create()
    {
        return new IoDeviceEmulator();
    }
protected:
    IoDeviceEmulator(){}
private:
    static BossnFactoryRegistrator<IoDeviceEmulator> registrator;

    virtual       QIODevice * internalGetDevice() { return nullptr; }
    virtual const QIODevice * internalGetDevice() const { return nullptr; }

};






#endif // IODEVICEEMULATOR_H
