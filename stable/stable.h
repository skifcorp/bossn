#ifndef STABLE_H
#define STABLE_H

#include <QSharedPointer>
#include <QVariant>
#include <QQueue>

#include "basetask.h"
#include "tags.h"
#include "factory.h"

template <class T>
class FixedQueue : public QQueue<T>
{
public:
    FixedQueue(typename QQueue<T>::size_type s):max_size(s){}
    FixedQueue():max_size(0){}

    void enqueue(const T &t)
    {
        while ( this->size() > max_size )  {
            this->dequeue();
        }
        QQueue<T>::enqueue(t);
    }


    void setMaxSize(typename QQueue<T>::size_type s){max_size = s;}
    typename QQueue<T>::size_type maxSize() const {return max_size;}
private:
    typename QQueue<T>::size_type max_size;
};


typedef FixedQueue<QVariant> VariantFixedQueue;

class StableTask : public BaseTask
{
    Q_OBJECT
public:
    typedef QSharedPointer<StableTask> Pointer;

    StableTask(Tags & t):BaseTask("StableTask"), tags(t), is_stable(false), is_busy(false) {}
    ~StableTask() {}

    //virtual void exec();
    virtual void setSettings(const QVariantMap& );

    Q_INVOKABLE QVariant isStable(const QString&, AlhoSequence *) const {return is_stable;}

    static BaseTask * create(Tags& t, const QVariantMap& )
    {
        return new StableTask(t);
    }
    virtual bool busy()const;
protected:
    virtual void run();
private:

    Tags& tags;
    bool is_stable;

    QString controlled_tag_name;
    QString controlled_tag_func;

    VariantFixedQueue values;
    QVariant delta;
    //QString signaled_tag_name;
    //QString signaled_tag_func;
    bool is_busy;
    static BossnFactoryRegistrator<StableTask> registrator;
};

#endif // STABLE_H
