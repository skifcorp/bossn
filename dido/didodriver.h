#ifndef DIDODRIVER_H
#define DIDODRIVER_H

#include <QSharedPointer>
#include <QMap>

#include "porterdriver.h"
#include "iodevicewrapper.h"

class DidoDriver : public PorterDriver
{
    Q_OBJECT
public:
    typedef QSharedPointer<DidoDriver> Pointer;

    ~DidoDriver() {}

    Q_INVOKABLE QVariant getDi(IoDeviceWrapper::Pointer::Type* io, int num);
    Q_INVOKABLE void setDo(IoDeviceWrapper::Pointer::Type* io, int num, bool);

    static PorterDriver* create(const QMap<QString, QVariant>& )
    {
        return new DidoDriver();
    }
protected:
    DidoDriver()
    {

    }

private:
    static bool registered;
    static bool registerInFact()
    {
        factory_map().insert("DidoDriver", &DidoDriver::create);
        return true;
    }

};

#endif // DIDODRIVER_H
