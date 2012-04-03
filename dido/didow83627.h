#ifndef DIDODRIVER_H
#define DIDODRIVER_H

#include <QSharedPointer>
#include <QMap>

#include "porterdriver.h"
#include "iodevicewrapper.h"

class DidoW83627 : public PorterDriver
{
    Q_OBJECT
public:
    typedef QSharedPointer<DidoW83627> Pointer;

    ~DidoW83627() {}

    Q_INVOKABLE QVariant getDi();
    Q_INVOKABLE void setDo(const QVariant& );

    Q_INVOKABLE QVariant getDiBit(const QVariant& full_byte, const QVariant& num);
    Q_INVOKABLE QVariant setDoBit(const QVariant& full_byte, const QVariant& num, const QVariant& );

    static PorterDriver* create(const QMap<QString, QVariant>& )
    {
        return new DidoW83627();
    }
protected:
    DidoW83627()
    {

    }

private:
    /*static bool registered;
    static bool registerInFact()
    {
        factory_map().insert("DidoIt8718f", &DidoIt8718f::create);
        return true;
    }*/
    static BossnFactoryRegistrator<DidoW83627> registator;
    uchar readAll( );

};

#endif // DIDODRIVER_H
