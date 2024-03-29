#ifndef WEIGHTDEVICE_H
#define WEIGHTDEVICE_H

#include <QMap>
#include <QVariant>
#include <QString>
#include <QSharedPointer>
#include <QDebug>

#include <functional>

using std::function;

#include "factory.h"

#if defined (Q_OS_WIN)
#include <windows.h>
#endif


class IoDeviceWrapper : public QObject,
                        public BossnFactory<IoDeviceWrapper>
{
    Q_OBJECT
public:
    typedef QSharedPointer<IoDeviceWrapper> Pointer;


    IoDeviceWrapper(const IoDeviceWrapper&) = delete;
    virtual ~IoDeviceWrapper() {}

    virtual void setSettings(const QMap<QString, QVariant>& ) = 0;
    virtual QString deviceName() const = 0;
    virtual void clear() = 0;


    virtual bool open(QIODevice::OpenModeFlag f)
    {
        bool is_opened = internalGetDevice()->open(f);
        return is_opened;
    }

    qint64 virtual write ( const QByteArray& data )
    {
        quint64 ret = internalGetDevice()->write(data);
        return ret;
    }

    qint64 virtual bytesAvailable () const
    {
        return internalGetDevice()->bytesAvailable();
    }
    QByteArray virtual read ( qint64 maxSize )
    {
        return internalGetDevice()->read(maxSize);
    }
    QByteArray virtual readAll ()
    {
        return internalGetDevice()->readAll();
    }

    QByteArray virtual peek ( qint64 maxSize )
    {
        return internalGetDevice()->peek(maxSize);
    }
    bool isDeviceOpened() const {return is_opened;}
#if defined(Q_OS_WIN)
    virtual BOOL DeviceIoControl(DWORD , LPVOID , DWORD , LPVOID , DWORD , LPDWORD ,  LPOVERLAPPED ) {return false; }
    virtual QString lastError() {return QString();}
#elif defined (Q_OS_LINUX)
    virtual int ioctl(int command, void * arg) {return 0;}
#endif

    static Pointer create(const QString& n)
    {
        if (factory_map().contains(n))  {
            Pointer p (factory_map()[n]());
            p->connectSignals();
            return p;
        }
        qWarning() << "IODeviceWrapper factory dont contains: " << n  << " class";
        return Pointer();
    }
public slots:
    void emulateReadyRead() {emit readyRead();}
protected:
    IoDeviceWrapper():is_opened(false) {}
    bool is_opened;
private:

    virtual QIODevice * internalGetDevice() = 0;
    virtual const QIODevice * internalGetDevice() const = 0;

    virtual void connectSignals()
    {
        if ( internalGetDevice() )
            connect(internalGetDevice(), SIGNAL(readyRead()), this, SIGNAL(readyRead()));
    }
signals:
    void readyRead();
};

#endif // WEIGHTDEVICE_H
