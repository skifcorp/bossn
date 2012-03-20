#ifndef IODEVICEDISCRETE_H
#define IODEVICEDISCRETE_H

#include <QIODevice>

#ifdef Q_OS_WIN32

#include <windows.h>

class DioDevice : public QIODevice
{
public:
    DioDevice();
    ~DioDevice();
    virtual bool open(QIODevice::OpenModeFlag );
    virtual void close();
protected:
    virtual qint64	readData ( char * data, qint64 maxSize );
    virtual qint64	writeData ( const char * data, qint64 maxSize );
private:
    HANDLE handle;
};

#else

class DioDevice : public QIODevice
{
public:
    DioDevice(){}
    ~DioDevice(){}
    virtual bool open(QIODevice::OpenModeFlag ) {return false;}
    virtual void close() {}
protected:
    virtual qint64	readData ( char * data, qint64 maxSize ) {return 0ul;}
    virtual qint64	writeData ( const char * data, qint64 maxSize ) {return 0ul;}

};


#endif

class IoDeviceDiscrete
{
public:
private:
    virtual QIODevice * internalGetDevice() {return &device;}
    virtual const QIODevice * internalGetDevice() const {return &device;}
    DioDevice device;
};

#endif
