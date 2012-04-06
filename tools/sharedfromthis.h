#ifndef SHARED_FROM_THIS_H
#define SHARED_FROM_THIS_H

#include <QSharedPointer>
#include <QWeakPointer>


template <class T>
class SharedFromThis
{
public:
    typedef QSharedPointer<T> Pointer;
    typedef QWeakPointer<T> WeakPointer;

    //SharedFromThis(){}
    //virtual ~SharedFromThis(){}

    Pointer sharedFromThis()
    {
        if (weak_ptr.isNull() ) {
            Pointer ptr(static_cast<T*>(this));
            //QSharedPointer<T> ptr(this);
            weak_ptr = ptr;
        }
        return weak_ptr.toStrongRef();
    }

    //Pointer sharedFromThis() const {return Pointer();}
private:
    WeakPointer weak_ptr;
};

#endif // SHARED_FROM_THIS_H
