#ifndef IODEVICECONSOLE_H
#define IODEVICECONSOLE_H


#include <QFile>
#include "iodevicewrapper.h"
#include "settingstool.h"

class IoDeviceConsole : public IoDeviceWrapper
{
public:

    virtual void setSettings(const QMap<QString, QVariant>& ) {}
    virtual QString deviceName() const {return QString();}

    virtual void clear() {}


    virtual bool open(QIODevice::OpenModeFlag )
    {
        is_opened = file_cin.open(stdin_num, QIODevice::ReadOnly);
        if (!is_opened ) {
            qWarning() << "cant open cin num: " << stdin_num; qFatal("Exiting...");
        }

        is_opened = file_cout.open(stdout_num, QIODevice::WriteOnly);
        if (!is_opened ) {
            qWarning() << "cant open cout num: " << stdout_num; qFatal("Exiting...");
        }
        return is_opened;
    }

    qint64 virtual write ( const QByteArray& data )
    {
        quint64 ret = file_cout.write(data.toPercentEncoding(QByteArray(), QByteArray(), ' '));
        return ret;
    }

    qint64 virtual bytesAvailable () const
    {
        return file_cin.bytesAvailable();
    }
    QByteArray virtual read ( qint64 maxSize )
    {
        return file_cin.read(maxSize);
    }
    QByteArray virtual readAll ()
    {
        return file_cin.readAll();
    }

    QByteArray virtual peek ( qint64 maxSize )
    {
        return file_cin.peek(maxSize);
    }

    static IoDeviceWrapper * create()
    {
        return new IoDeviceConsole();
    }
protected:
    IoDeviceConsole(){}
private:
    QFile file_cin;
    QFile file_cout;

    static BossnFactoryRegistrator<IoDeviceConsole> registrator;

    virtual       QIODevice * internalGetDevice() { return nullptr; }
    virtual const QIODevice * internalGetDevice() const { return nullptr; }

    static const int stdin_num  = 0;
    static const int stdout_num = 1;
};


#endif

