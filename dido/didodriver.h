#ifndef DIDODRIVER_H
#define DIDODRIVER_H

#include <QSharedPointer>
#include <QMap>

#include "porterdriver.h"
#include "iodevicewrapper.h"

class DidoIt8718f : public PorterDriver
{
    Q_OBJECT
public:
    typedef QSharedPointer<DidoIt8718f> Pointer;

    ~DidoIt8718f() {}

    Q_INVOKABLE QVariant getDi(IoDeviceWrapper::Pointer::Type* io, int num);
    Q_INVOKABLE void setDo(IoDeviceWrapper::Pointer::Type* io, int num, bool);

    static PorterDriver* create(const QMap<QString, QVariant>& )
    {
        return new DidoIt8718f();
    }
protected:
    DidoIt8718f()
    {

    }

private:
    static bool registered;
    static bool registerInFact()
    {
        factory_map().insert("DidoIt8718f", &DidoIt8718f::create);
        return true;
    }
    uchar readAll(IoDeviceWrapper::Pointer::Type* );

};

#endif // DIDODRIVER_H
