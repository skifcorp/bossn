#ifndef WEIGHTDEVICEFILE_H
#define WEIGHTDEVICEFILE_H

#include <QFile>
#include "iodevicewrapper.h"
#include "settingstool.h"

class IoDeviceFile : public IoDeviceWrapper
{
public:
    ~IoDeviceFile() {}

    virtual void setSettings(const QMap<QString, QVariant>& s)
    {        
        file.setFileName( get_setting<QString>("fileName", s) );
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
        return new IoDeviceFile();
    }

    qint64 virtual write ( const QByteArray& data )
    {       
       file.seek(0);
       //qDebug() << "write to file!!!! "<<data[0];
       qint64 ret = IoDeviceWrapper::write(data);
       file.flush();
       return ret;
    }

    QByteArray virtual read ( qint64 maxSize )
    {
        file.seek(0);
        return IoDeviceWrapper::read(maxSize);
    }
    QByteArray virtual readAll ()
    {
        file.seek(0);
        return IoDeviceWrapper::readAll();
    }
protected:
    IoDeviceFile(){}
private:
    QFile file;
    static BossnFactoryRegistrator<IoDeviceFile> registrator;

    virtual QIODevice * internalGetDevice() { return &file; }
    virtual const QIODevice * internalGetDevice() const { return &file; }

};


#endif // WEIGHTDEVICEFILE_H
