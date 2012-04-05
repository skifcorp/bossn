#ifndef SHARED_FROM_THIS_H
#define SHARED_FROM_THIS_H

#include <QSharedPointer>
#include <QWeakPointer>


template <class T>
class SharedFromThis
{
public:
    QSharedPointer<T> sharedFromThis()
    {
        if (weak_ptr.isNull() ) {
            QSharedPointer<T> ptr(this);
            weak_ptr = ptr;
        }
        return weak_ptr.toStrongRef();
    }
private:
    QWeakPointer<T> weak_ptr;
};

#endif // SHARED_FROM_THIS_H
