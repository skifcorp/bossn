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


    bool open(QIODevice::OpenModeFlag f)
    {
        return internalGetDevice()->open(f);
    }

    qint64 virtual write ( const QByteArray& data )
    {
        //qDebug() << "write !!!!";

        return internalGetDevice()->write(data);
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

    QByteArray peek ( qint64 maxSize )
    {
        return internalGetDevice()->peek(maxSize);
    }

#if defined(Q_OS_WIN)
    virtual BOOL DeviceIoControl(DWORD , LPVOID , DWORD , LPVOID , DWORD , LPDWORD ,  LPOVERLAPPED ) {return false; }
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

protected:
    //typedef QMap<QString, function<IoDeviceWrapper * ()> > FactoryMap;
    IoDeviceWrapper() {}
    /*static FactoryMap & factory_map()
    {
        static FactoryMap map;
        return map;
    }*/
private:
    virtual QIODevice * internalGetDevice() = 0;
    virtual const QIODevice * internalGetDevice() const = 0;

    void connectSignals()
    {
        connect(internalGetDevice(), SIGNAL(readyRead()), this, SIGNAL(readyRead()));
    }
signals:
    void readyRead();
};

#endif // WEIGHTDEVICE_H
