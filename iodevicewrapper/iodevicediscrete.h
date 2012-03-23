#ifndef IODEVICEDISCRETE_H
#define IODEVICEDISCRETE_H

#include <QIODevice>
#include <QString>
#include <QMap>

#include "iodevicewrapper.h"

#ifdef Q_OS_WIN32

#include <windows.h>

class DioDevice : public QIODevice
{
public:
    DioDevice();
    ~DioDevice();
    virtual bool open(QIODevice::OpenModeFlag );
    virtual void close();
    void setDeviceName(const QString& dn){device_name = dn;}
    QString deviceName() const {return device_name;}
protected:
    virtual qint64	readData ( char * data, qint64 maxSize );
    virtual qint64	writeData ( const char * data, qint64 maxSize );
private:
    HANDLE handle;
    QString device_name;
};

#else

class DioDevice : public QIODevice
{
public:
    DioDevice(){}
    DioDevice(const QString& dn):device_name(dn){}
    ~DioDevice(){}
    virtual bool open(QIODevice::OpenModeFlag ) {return false;}
    virtual void close() {}
    void setDeviceName(const QString& ){}
    QString deviceName() const {return QString();}
protected:
    virtual qint64	readData ( char * data, qint64 maxSize ) {return 0ul;}
    virtual qint64	writeData ( const char * data, qint64 maxSize ) {return 0ul;}
private:

};


#endif

class IoDeviceDiscrete : public IoDeviceWrapper
{
public:
    IoDeviceDiscrete () {}
    ~IoDeviceDiscrete () {}

    virtual void setSettings(const QMap<QString, QVariant>& s)
    {
        auto iter = s.find("deviceName");
        if (iter != s.end()) device.setDeviceName(iter->toString());
    }

    virtual QString deviceName() const
    {
        return device.deviceName();
    }
    virtual void clear()
    {

    }

    static IoDeviceWrapper * create()
    {
        return new IoDeviceDiscrete();
    }
protected:

private:
    virtual QIODevice * internalGetDevice() {return &device;}
    virtual const QIODevice * internalGetDevice() const {return &device;}
    DioDevice device;

    static bool registered;
    static bool registerInFact()
    {
        factory_map().insert("IoDeviceDiscrete", &IoDeviceDiscrete::create);
        return true;
    }
};

#endif
