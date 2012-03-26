#ifndef IODEVICEDISCRETE_H
#define IODEVICEDISCRETE_H

#include <QIODevice>
#include <QString>
#include <QMap>
#include <QSharedPointer>
#include <QFile>

#include <functional>

using std::function;

#include "iodevicewrapper.h"

/*
class DioLocalDriver : public QIODevice
{
public:
    typedef QSharedPointer<DioLocalDriver> Pointer;
    virtual void setDeviceName(const QString& ) = 0;
    virtual  QString deviceName() const = 0;
    static Pointer create(const QString& n)
    {
        if (factory_map().contains(n))  {
            Pointer p (factory_map()[n]());
            //qDebug () << "pointer: "<<p.data();
            return p;
        }
        qWarning() << "DioDevice factory dont contains: " << n  << " class";
        return Pointer();
    }
protected:
    typedef QMap<QString, function<DioLocalDriver * ()> > FactoryMap;

    static FactoryMap & factory_map()
    {
        static FactoryMap map;
        return map;
    }
private:

};*/


#ifdef Q_OS_WIN32

#include <windows.h>
#include <io.h>
/*
class DioDeviceIt8718f : public DioLocalDriver
{
public:

    ~DioDeviceIt8718f();
    virtual bool open(QIODevice::OpenModeFlag );
    virtual void close();
    void setDeviceName(const QString& dn){device_name = dn;}
    QString deviceName() const {return device_name;}
    static DioLocalDriver * create()
    {
        return new DioDeviceIt8718f();
    }
protected:
    DioDeviceIt8718f();
    virtual qint64	readData ( char * data, qint64 maxSize );
    virtual qint64	writeData ( const char * data, qint64 maxSize );
private:
    HANDLE handle;
    QString device_name;
    static bool registered;
    static bool registerInFact()
    {
        factory_map().insert("DioDeviceIt8718f", &DioDeviceIt8718f::create);
        return true;
    }
};*/

#endif

class HandleGuard
{
public:
    HandleGuard(HANDLE h):handle_(h){}
    ~HandleGuard(){CloseHandle(handle_);}
    HANDLE handle(){return handle_;}
private:
    HANDLE handle_;
};

class IoDeviceCtl : public IoDeviceWrapper
{

public:
    IoDeviceCtl () {}
    ~IoDeviceCtl () {}

    virtual void setSettings(const QMap<QString, QVariant>& s)
    {
        {
            auto iter = s.find("deviceName");
            if (iter != s.end()) file.setFileName(iter->toString());
        }
    }

    virtual QString deviceName() const
    {
        return file.fileName();
    }
    virtual void clear()
    {

    }

    static IoDeviceWrapper * create()
    {
        return new IoDeviceCtl();
    }
#if defined(Q_OS_WIN)
    virtual BOOL DeviceIoControl(DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize,
                                 LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned,
                                 LPOVERLAPPED lpOverlapped )
    {
        HandleGuard grd(reinterpret_cast<HANDLE> (_get_osfhandle(file.handle())));
        return ::DeviceIoControl(grd.handle(),  dwIoControlCode, lpInBuffer,  nInBufferSize, lpOutBuffer,  nOutBufferSize,  lpBytesReturned, lpOverlapped);
        return false;
    }

#elif defined (Q_OS_LINUX)
    virtual int ioctl(int command, void * arg) {return 0;}
#endif
protected:

private:
    virtual QIODevice * internalGetDevice()
    {
        return &file;
    }

    virtual const QIODevice * internalGetDevice() const
    {
        return &file;
    }
    QFile file;


    static bool registered;
    static bool registerInFact()
    {
        factory_map().insert("IoDeviceCtl", &IoDeviceCtl::create);
        return true;
    }
};

#endif
