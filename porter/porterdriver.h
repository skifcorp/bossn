#ifndef __WEIGHT_H__
#define __WEIGHT_H__

#include <functional>

using std::function;

#include <QObject>
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

class PorterDriver : public QObject
{    
    Q_OBJECT
public:		
    typedef QSharedPointer<PorterDriver> Pointer;

    enum WeightError {
        WeightFrameOk = 0, WeightFrameCorrupted, WeightFrameBadConf, WeightFrameBadAddress, WeightFrameNotAnswer
    };

    virtual ~PorterDriver(){}


    static Pointer create(const QString& n, const QMap<QString, QVariant>& drv_conf )
    {
        if (factory_map().contains(n))
            return Pointer(factory_map()[n](drv_conf));

        qWarning() << "WeightDriver factory dont contains: "<<n<< " class";
        return Pointer();
    }

protected:
    PorterDriver(){}

    typedef QMap<QString, function<PorterDriver * (const QMap<QString, QVariant>)> > FactoryMap;

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
