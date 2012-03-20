#ifndef __WEIGHT_H__
#define __WEIGHT_H__

#include <functional>

using std::function;


#include <QMap>
#include <QSharedPointer>
#include <QVariant>


#include "coroutine.h"
#include "iodevicewrapper.h"

/*
class WeightFrameException
{
public:
    WeightFrameException() {}
    virtual ~WeightFrameException() {}
};

class WeightFrameExceptionTooSmall : public WeightFrameException
{
public:
    WeightFrameExceptionTooSmall(){}
};

class WeightFrameExceptionCorrupted : public WeightFrameException
{
public:
    WeightFrameExceptionCorrupted(){}
};

class WeightFrameExceptionBadConf : public WeightFrameException
{
public:
    WeightFrameExceptionBadConf(){}
};

class WeightFrameExceptionBadAddress : public WeightFrameException
{
public:
    WeightFrameExceptionBadAddress(){}
};*/

class WeightDriver
{    
public:		
    typedef QSharedPointer<WeightDriver> Pointer;

    enum WeightError {
        WeightFrameOk = 0, WeightFrameCorrupted, WeightFrameBadConf, WeightFrameBadAddress, WeightFrameNotAnswer
    };

    virtual ~WeightDriver(){}

    virtual void readWeight(IoDeviceWrapper *, float &, uint & ) {}
    virtual void zero(IoDeviceWrapper *, uint &) {}

    static Pointer create(const QString& n, const QMap<QString, QVariant>& drv_conf )
    {
        if (factory_map().contains(n))
            return Pointer(factory_map()[n](drv_conf));

        qWarning() << "WeightDriver factory dont contains: "<<n<< " class";
        return Pointer();
    }

protected:
    WeightDriver(){}

    typedef QMap<QString, function<WeightDriver * (const QMap<QString, QVariant>)> > FactoryMap;

    static FactoryMap & factory_map()
    {
        static FactoryMap map;
        return map;
    }
    void yield()
    {
        Coroutine::yield();
    }

};

#endif
