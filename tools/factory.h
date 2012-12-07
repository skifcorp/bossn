#ifndef FACTORY_H
#define FACTORY_H

#include <QSharedPointer>
#include <QMap>
#include <QDebug>

#include "functional"
#include "type_traits"

using std::function;
using std::conditional;
using std::is_base_of;

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
    template <class T1, class T2> friend class BossnFactoryRegistrator2_impl;
public:
    using Pointer  = QSharedPointer<T>;
    using ThisType = BossnFactory<T, Args...>;

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


//class YouMustInheritFromBossnFactory;

/*template <class T>
struct BossnFactoryRegistrator2 :
        conditional< is_base_of< BossnFactory<T>, T >::value, BossnFactoryRegistrator2<T::ThisType>, YouMustInheritFromBossnFactory >
{

};*/

template <class T, class T2>
struct BossnFactoryRegistrator2_impl;

template <class Derived, class Base, class ... Args>
class BossnFactoryRegistrator2_impl< Derived, BossnFactory<Base, Args ...> >
{
public:
    BossnFactoryRegistrator2_impl(const QString& class_name)
    {
        Base::factory_map().insert(class_name, create);
    }

    static Base * create ( Args ... args  )
    {
        return new Derived(args...);
    }

};



template <class T>
struct BossnFactoryRegistrator2 : BossnFactoryRegistrator2_impl<T, typename T::ThisType>
{
    BossnFactoryRegistrator2(const QString& n) : BossnFactoryRegistrator2_impl<T, typename T::ThisType>(n) {}
};





#endif // FACTORY_H
