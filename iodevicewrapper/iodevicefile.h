#ifndef WEIGHTDEVICEFILE_H
#define WEIGHTDEVICEFILE_H

#include <QFile>
#include "iodevicewrapper.h"

class IoDeviceFile : public IoDeviceWrapper
{
public:
    ~IoDeviceFile() {}

    virtual void setSettings(const QMap<QString, QVariant>& s)
    {
        auto iter = s.find("fileName");
        if (iter != s.end()) file.setFileName(iter->toString());
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

protected:
    IoDeviceFile(){}
private:
    QFile file;
    static bool registered;
    static bool registerInFact()
    {
        factory_map().insert("IoDeviceFile", &IoDeviceFile::create);
        return true;
    }

    virtual QIODevice * internalGetDevice() { return &file; }
    virtual const QIODevice * internalGetDevice() const { return &file; }
};


#endif // WEIGHTDEVICEFILE_H
