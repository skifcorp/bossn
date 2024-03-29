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


#include "boost/mpl/string.hpp"

namespace mpl = boost::mpl;

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
    template <class T1, class T2, class N> friend class BossnFactoryRegistrator2_impl;
public:
    using Pointer  = QSharedPointer<T>;
    using ThisType = BossnFactory<T, Args...>;
    using FuncType = function<T * (Args...)>;

    static Pointer create(const QString& n, Args... args)
    {       
        auto iter = factory_map().find(n);
        if (iter == factory_map().end()) {
            qWarning()<<"Factory cant find class with name: "<<n; return Pointer();
        }
        return Pointer((*iter)(args ...));
    }
    static bool insertFunc(const QString& n, FuncType && f)
    {
        factory_map().insert(n, f); return true;
    }

protected:
    typedef QMap<QString,  FuncType> FactoryMap;
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

template <class T, class T2, class N>
struct BossnFactoryRegistrator2_impl;

template <class Derived, class Base, int ... N, class ... Args>
class BossnFactoryRegistrator2_impl< Derived, BossnFactory<Base, Args ...>, mpl::string<N...> >
{
public:
    BossnFactoryRegistrator2_impl()
    {
        //Base::factory_map().insert(mpl::c_str<mpl::string<N...>>::value, create);
        Base::insertFunc(mpl::c_str<mpl::string<N...>>::value, create);
    }

    static Base * create ( Args ... args  )
    {
        return new Derived(args...);
    }

};



template <class T, class N>
struct BossnFactoryRegistrator2 : BossnFactoryRegistrator2_impl<T, typename T::ThisType, N>
{
    using ParentType = BossnFactoryRegistrator2_impl<T, typename T::ThisType, N>;
    BossnFactoryRegistrator2() : ParentType() {}
};





#endif // FACTORY_H
