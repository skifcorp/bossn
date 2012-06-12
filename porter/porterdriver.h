#ifndef __WEIGHT_H__
#define __WEIGHT_H__

#include <functional>

using std::function;

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include <QVariant>
#include <QWeakPointer>

#include "coroutine.h"
#include "iodevicewrapper.h"
#include "factory.h"
#include "iodevicewrapper.h"

class PorterDriver : public QObject,
                     public BossnFactory<PorterDriver, const QMap<QString, QVariant> >
{    
    Q_OBJECT
public:		
    typedef QSharedPointer<PorterDriver> Pointer;
    typedef QWeakPointer<IoDeviceWrapper::Pointer::Type> DevPointer;

    enum PorterError {
        PorterFrameOk = 0, PorterFrameCorrupted, PorterFrameBadConf, PorterFrameBadAddress, PorterFrameNotAnswer
    };

    virtual ~PorterDriver(){}

    void setIoDevice(DevPointer io) {io_device_ = io;}
    IoDeviceWrapper::Pointer::Type* io_device() {return io_device_.toStrongRef().data();}
protected:
    DevPointer io_device_;
    PorterDriver(){}

    void yield()
    {
        Coroutine::yield();
    }

};

#endif
