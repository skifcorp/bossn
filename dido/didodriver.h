#ifndef DIDODRIVER_H
#define DIDODRIVER_H

#include <QSharedPointer>
#include <QMap>

#include <functional>

using std::function;

class DidoDriver
{
public:
    typedef QSharedPointer<DidoDriver> Pointer;

    virtual ~DidoDriver();

    virtual bool getDi(int num);
    virtual void setDo(int num, bool);

    Pointer create(const QString& n)
    {
        return Pointer(factory_map()[n]());
    }



protected:
    typedef QMap<QString, function<DidoDriver * (const QMap<QString, QVariant>)> > FactoryMap;
    DidoDriver()
    {

    }

    static FactoryMap & factory_map()
    {
        static FactoryMap map;
        return map;
    }

};

#endif // DIDODRIVER_H
