#ifndef FACTORY_H
#define FACTORY_H

#include <QSharedPointer>
#include <QMap>
#include <QDebug>
#include "functional"
using std::function;

template <class T>
class BossnFactoryRegistrator
{
public:
    BossnFactoryRegistrator(const QString& class_name)
    {
        T::factory_map().insert(class_name, &T::create);
    }
};

template <class T, class ... Args>
class BossnFactory
{
    template <class C> friend class BossnFactoryRegistrator;
public:
    typedef QSharedPointer<T> Pointer;

    static Pointer create(const QString& n, Args... args)
    {       
        auto iter = factory_map().find(n);
        if (iter == factory_map().end()) {
            qWarning()<<"Factory cant find class with name: "<<n; return Pointer();
        }
        return Pointer((*iter)(args ...));
    }
protected:
    typedef QMap<QString, function<T * (Args...)> > FactoryMap;
    static FactoryMap& factory_map()
    {
        static FactoryMap map;
        return map;
    }
};


#endif // FACTORY_H
