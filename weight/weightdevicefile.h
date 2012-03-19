#ifndef WEIGHTDEVICEFILE_H
#define WEIGHTDEVICEFILE_H

#include <QFile>
#include "weightdevice.h"

class WeightDeviceFile : public WeightDevice
{
public:
    ~WeightDeviceFile() {}

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

    static WeightDevice * create()
    {
        return new WeightDeviceFile();
    }

protected:
    WeightDeviceFile(){}
private:
    QFile file;
    static bool registered;
    static bool registerInFact()
    {
        factory_map().insert("WeightDeviceFile", &WeightDeviceFile::create);
        return true;
    }

    virtual QIODevice * internalGetDevice() { return &file; }
    virtual const QIODevice * internalGetDevice() const { return &file; }
};


#endif // WEIGHTDEVICEFILE_H
