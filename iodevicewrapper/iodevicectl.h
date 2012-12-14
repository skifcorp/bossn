#ifndef IODEVICEDISCRETE_H
#define IODEVICEDISCRETE_H

#include <QIODevice>
#include <QString>
#include <QMap>
#include <QSharedPointer>
#include <QFile>
#include <QTextCodec>

#include <functional>

using std::function;

#include "iodevicewrapper.h"
#include "factory.h"




#ifdef Q_OS_WIN32

#include <windows.h>
#include <io.h>

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
    ~IoDeviceCtl ()
    {
        if (hndFile != INVALID_HANDLE_VALUE)
            CloseHandle(hndFile);
    }

    virtual void setSettings(const QMap<QString, QVariant>& s)
    {
        {
            auto iter = s.find("deviceName");
            if (iter != s.end()) {
                file_name = iter->toString();
            }
        }
    }

    virtual QString deviceName() const
    {
        return file_name;
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
        //HandleGuard grd(reinterpret_cast<HANDLE> (_get_osfhandle(file.handle())));
        return ::DeviceIoControl(hndFile,  dwIoControlCode, lpInBuffer,  nInBufferSize, lpOutBuffer,  nOutBufferSize,  lpBytesReturned, lpOverlapped);
    }
    virtual QString lastError()
    {
        char lpMsgBuf[1000];

        DWORD dw = GetLastError();
        if (dw) qDebug() << "error code: " <<dw;
        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (char *)&lpMsgBuf,
            1000, NULL );

        QTextCodec * c = QTextCodec::codecForName("IBM866");

        QString s = c->toUnicode(lpMsgBuf);
        return s;
    }

#elif defined (Q_OS_LINUX)
    virtual int ioctl(int command, void * arg) {return 0;}
#endif
    virtual bool open(QIODevice::OpenModeFlag )
    {
        hndFile = CreateFile(L"\\\\.\\WDT_DEVICE",
                             GENERIC_READ | GENERIC_WRITE,
                             FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
         is_opened = hndFile != INVALID_HANDLE_VALUE;
         return is_opened;
    }


protected:

private:
    QString file_name;
    HANDLE hndFile;

    virtual QIODevice * internalGetDevice()
    {
        return nullptr;
    }

    virtual const QIODevice * internalGetDevice() const
    {
        return nullptr;
    }


    static BossnFactoryRegistrator<IoDeviceCtl> registrator;
};

#endif
