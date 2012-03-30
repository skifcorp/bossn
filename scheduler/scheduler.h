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

struct Schedul
{
    typedef QSharedPointer<QTimer> TimerPointer;

    function<void ()> schedule_func;
    function<void ()> timeout_func;
    TimerPointer schedule_timer;
    TimerPointer timeout_timer;

    Schedul( function<void ()> schf, function<void ()> tmf, int schedul_msec, int timeout_msec)
        :schedule_func(schf), timeout_func(tmf), schedule_timer(new QTimer), timeout_timer(new QTimer), num(0)
    {
        schedule_timer->setSingleShot(true);
        schedule_timer->setInterval(schedul_msec);

        timeout_timer->setSingleShot(true);
        timeout_timer->setInterval(timeout_msec);
    }
    int num;
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
    CoroContext(const CoroPointer & c, Schedul *s):coro(c), schedul(s) {}
    CoroContext():schedul(nullptr) {}
    void clear()
    {
        schedul = nullptr;
        coro.clear();
    }
    bool empty() const {return schedul == nullptr && !coro.data();}
};

class Scheduler : public QObject
{
    Q_OBJECT
public:    
    typedef QWeakPointer<IoDeviceWrapper> IoDevPointer;

    Scheduler ();
    ~Scheduler(){}

    void addFunction( function<void ()>, function<void ()>, int schedul_msec, int timeout_msec );
    void execFunction( function<void ()>, function<void ()>, int timeout_msec );

    void setDevice(IoDevPointer d);
    void clear();
    bool busy() const {return !current_coro.empty();}
    void waitForFree () const {   while (busy()) {qApp->processEvents();}   }

private slots:
    void onTimeoutTimer();
    void onScheduleTimer(Schedul & s);
    void onReadyRead();
private:    
    template <class R, class T>
    friend void connect_callable(QObject * sender, const char * signal, R*receiver , const T& slot,  Qt::ConnectionType t = Qt::AutoConnection);

    typedef QList<Schedul> Scheduls;

    CoroContext        current_coro;
    IoDevPointer       device;
    QObjectList        conn_obj;

    Scheduls scheduls;


    void execute();
    void startNewCoro(Schedul &);
};

#endif // SCHEDULER_H
