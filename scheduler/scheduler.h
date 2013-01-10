#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QTimer>
#include <QCoreApplication>

#include <functional>

using std::function;

#include "coroutine.h"


class IoDeviceWrapper;
class Scheduler;
class AlhoSequence;

class Schedul : public QObject

{
    Q_OBJECT
public:
    typedef QSharedPointer<Schedul> Pointer;

    Schedul(Scheduler & s, function<void ()> schf, function<void ()> tmf, int schedul_msec, int timeout_msec);
    ~Schedul();

    int num;
    Schedul(const Schedul& ) = delete;

    function<void ()> schedule_func;
    function<void ()> timeout_func;
    QTimer timeout_timer;

    void startScheduleTimer(const QString& );
    static void coro_deleter(Coroutine *);
    static void coro_deleter_for_external_coro(Coroutine *);

    void setExternalCoro(Coroutine *);
    void yield()
    {
        coro->yield();
    }
    bool cont()
    {
        return coro->cont();
    }
    bool running() const
    {
        return coro->status() == Coroutine::Stopped;
    }

    Coroutine::Status status() const
    {
        return coro->status();
    }

protected:
    //virtual void run();
private:
    typedef QSharedPointer<Coroutine> TrickyCoroPointer;

    Scheduler & scheduler;



    QTimer schedule_timer;
    TrickyCoroPointer coro;

public slots:
    void onScheduleTimer();    
    void onSchedulerFinished();
};

class connect_helper : public QObject
{
    Q_OBJECT
public:
    connect_helper(QObjectList& l, function<void ()> f):func(f)
    {
        l.push_back(this);
    }
public slots:
    void signaled() {func();}
private:
    function<void ()> func;
};

template <class R, class T>
void connect_callable(QObject * sender, const char * signal, R*receiver , const T& slot,  Qt::ConnectionType t = Qt::AutoConnection)
{
    QObject::connect(sender, signal, new connect_helper(receiver->conn_obj,slot), SLOT(signaled()));
}

struct CoroContext
{
    typedef QSharedPointer<Coroutine> CoroPointer;
    CoroPointer coro;
    Schedul * schedul;
    CoroContext(const CoroPointer & c, Schedul *s):coro(c), schedul(s) , activate_on_finish(false), cyclic(true){}
    CoroContext():schedul(nullptr), activate_on_finish(false), cyclic(true) {}
    void clear();
    bool empty() const {return schedul == nullptr && !coro.data();}
    bool activate_on_finish;
    bool cyclic;
    QString tag_name;
    QString func_name;
};

class Scheduler : public QObject
{
    friend class Schedul;

    Q_OBJECT
signals:
    void finished();
    void finished2();
public:    
    typedef QWeakPointer<IoDeviceWrapper> IoDevPointer;

    Scheduler ();
    ~Scheduler(){}

    void addFunction( function<void ()>, function<void ()>, int schedul_msec, int timeout_msec, const QString& func_name );

    void execFunction(AlhoSequence * caller, function<void ()>, function<void ()>, int timeout_msec,
                      const QString& tag_name, const QString& func_name);

    void setDevice(IoDevPointer d);
    void clear();
    bool busy() const {return !current_coro.empty();}
public slots:

    void waitForFree (Schedul&, bool important );

protected:

private slots:
    void onTimeoutTimer();
    //void onScheduleTimer(Schedul & s);
    void onReadyRead();
    //void continueExec();
private:    
    template <class R, class T>
    friend void connect_callable(QObject * sender, const char * signal, R*receiver , const T& slot,  Qt::ConnectionType t = Qt::AutoConnection);

    typedef QList<Schedul::Pointer> Scheduls;

    CoroContext        current_coro;
    IoDevPointer       device;
    QObjectList        conn_obj;

    Scheduls scheduls;

    QList<QObject*> waiters;


    void execute();
    void startNewCoro(Schedul &, bool important, bool actiavate_on_finish, bool cyclic, const QString&,
                      const QString& );
    void activateWaiter();

};

#endif // SCHEDULER_H
