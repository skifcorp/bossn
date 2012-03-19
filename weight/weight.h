#ifndef __WEIGHT_H__
#define __WEIGHT_H__

#include <functional>

using std::function;


#include <QMap>
#include <QSharedPointer>
#include <QVariant>


#include "coroutine.h"
#include "weightdevice.h"

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
        WeightFrameOk = 0, WeightFrameCorrupted, WeightFrameBadConf, WeightFrameBadAddress
    };

    virtual ~WeightDriver(){}

    virtual void readWeight(WeightDevice *, float &, uint & ) {}
    virtual void zero(WeightDevice *, uint &) {}

    static Pointer create(const QString& n, const QMap<QString, QVariant>& drv_conf )
    {
        return Pointer(factory_map()[n](drv_conf));
    }

protected:
    WeightDriver(){}

    typedef QMap<QString, function<WeightDriver * (const QMap<QString, QVariant>)> > FactoryMap;

    static FactoryMap & factory_map()
    {
        static FactoryMap map;
        return map;
    }

};

#endif
