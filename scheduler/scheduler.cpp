#include <QDebug>

#include "scheduler.h"
#include "iodevicewrapper.h"
#include "coroutine.h"
#include "alhosequence.h"

#include <QCoreApplication>

#include <boost/bind.hpp>

#include "deleterlater.h"

using CoroDeleter = DeleterLater<Coroutine>;

void CoroContext::clear()
{
    coro.clear();

    if (activate_on_finish && schedul->running()) {
        //qDebug() << "!!!!!!!!!!!!!GOT activate_on_finish!!!!: status: ";
        schedul->cont();
    }

    schedul = nullptr;

    tag_name = func_name = QString();
}

Schedul::Schedul(Scheduler & s, function<void ()> schf, function<void ()> tmf, int schedul_msec, int timeout_msec)
    :num(0), schedule_func(schf), timeout_func(tmf), scheduler(s)
{
    schedule_timer.setSingleShot(true);
    schedule_timer.setInterval(schedul_msec);

    timeout_timer.setSingleShot(true);
    timeout_timer.setInterval(timeout_msec);

    connect(&schedule_timer,SIGNAL(timeout()), this, SLOT(onScheduleTimer()) );
}

Schedul::~Schedul()
{

}

void Schedul::coro_deleter(Coroutine *c)
{
    CoroDeleter * deleter = new CoroDeleter(c);
    deleter->deleteLater();
}

void Schedul::coro_deleter_for_external_coro(Coroutine *)
{
    //does nothing
}

void Schedul::onScheduleTimer()
{
    coro->cont();
}

void Schedul::onSchedulerFinished()
{
    coro->cont();
}

void Schedul::startScheduleTimer(const QString& func_name)
{
    schedule_timer.start();

    coro = TrickyCoroPointer (
                Coroutine::build(
                    [this, func_name] {
                        scheduler.startNewCoro(*this, false, false, true,
                                                 "cyclic", func_name);
                    },
                    "coro_for: " + func_name
                ),
                coro_deleter );

    coro->createStack(32768 * 4);
}

void Schedul::setExternalCoro(Coroutine * c)
{
    coro = TrickyCoroPointer(c, coro_deleter_for_external_coro);
}

Scheduler::Scheduler ()
{

}

void Scheduler::setDevice(IoDevPointer d)
{
    if (!device.isNull()) {
        device.toStrongRef().data()->disconnect(SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        clear();
    }
    device = d;
    connect(device.toStrongRef().data(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}


void Scheduler::clear()
{
    current_coro.clear();
    scheduls.clear();
    conn_obj.clear();
}

void Scheduler::addFunction(function<void ()>schf, function<void ()> tmf, int sch_msec, int tm_msec, const QString& func_name)
{
    scheduls.push_back( Schedul::Pointer( new Schedul(*this, schf, tmf, sch_msec, tm_msec)));
    Schedul & s = *scheduls.back();
    s.num = scheduls.count() - 1;
    //connect_callable(s.schedule_timer.data(), SIGNAL(timeout()), this, bind(&Scheduler::onScheduleTimer, this, std::ref(s) ));
    connect(&s.timeout_timer, SIGNAL(timeout()), this, SLOT(onTimeoutTimer()));

    s.startScheduleTimer(func_name);
}

void Scheduler::waitForFree (Schedul& s, bool important)
{
    if (!busy()) {
        return;
    }

    if (important)
        waiters.push_front(&s);
    else
        waiters.push_back(&s);

    while (busy()) {
        s.yield();
    }

    waiters.removeAll(&s);
}

void Scheduler::activateWaiter()
{
    if (waiters.isEmpty()) return;

    QTimer::singleShot(0, waiters.first(), SLOT(onSchedulerFinished()) );
}

void Scheduler::execFunction(AlhoSequence * caller, function<void ()>schf, function<void ()> tmf,
               int tm_msec, const QString& tag_name, const QString& func_name)
{
    Schedul s(*this, schf, tmf, 0, tm_msec);
    s.setExternalCoro(caller);
    connect(&s.timeout_timer, SIGNAL(timeout()), this, SLOT(onTimeoutTimer()));

    startNewCoro(s, true, true, false, tag_name, func_name);

//    qDebug() << "2: !!!!!!!!! exec function: afterStartNewCoro!!!!: " << caller->objectName()
//             << " func_name: " << current_coro.func_name;

    while (current_coro.coro && current_coro.coro->status() == Coroutine::Stopped ) {
//        qDebug() << "before yeild scheduler "<< tag_name << " func: "<< func_name <<  " wants to sleep! because " << current_coro.tag_name << " "
//                 << current_coro.func_name << " works" ;
        s.yield();
        //qDebug() << "scheduler wake up ... status: " <<
        if (current_coro.coro && current_coro.coro->status() == Coroutine::Stopped) {
            qWarning() << "ATTANTION!!! Bad wakeup for: " << current_coro.tag_name << " "
                       << " func: " << current_coro.func_name;
        }
    }

//    qDebug () << "3: !!!!!!!!!!! URAAA exec function! exit for : " << caller->objectName();
}


void Scheduler::onTimeoutTimer()
{    
    current_coro.schedul->timeout_func();

    device.data()->clear();

    if (current_coro.cyclic )
        current_coro.schedul->startScheduleTimer( current_coro.coro->coroName() );

    current_coro.clear();

    activateWaiter();
}

void Scheduler::startNewCoro(Schedul & s, bool important, bool activate_on_finish, bool cyclic, const QString& tag_name,
                             const QString& func_name)
{
    waitForFree(s, important);

    current_coro = CoroContext(QSharedPointer<Coroutine>(  Coroutine::build( s.schedule_func, func_name ) ), &s);
    current_coro.activate_on_finish = activate_on_finish;
    current_coro.cyclic = cyclic;
    current_coro.coro->createStack(65535*4);
    current_coro.tag_name = tag_name;
    current_coro.func_name = func_name;

    execute();
}

void Scheduler::onReadyRead()
{
    execute();
}

void Scheduler::execute()
{
    if (!current_coro.schedul) {
        qWarning() << "\n\n\nsomething terrible! GOT EVENT ON PORT WHILE NOTHING WRITED!! "<<device.data()->deviceName()<<"bytes num:"
                   << device.data()->bytesAvailable() << "\n\n\n";
        device.data()->clear();
        return;
    }

    current_coro.schedul->timeout_timer.start();

    current_coro.coro->cont();

    Coroutine::Status s = current_coro.coro->status();

    if (s == Coroutine::NotStarted || s == Coroutine::Terminated ) {
        current_coro.schedul->timeout_timer.stop();
        if (current_coro.cyclic)
            current_coro.schedul->startScheduleTimer( current_coro.coro->coroName() );        
        current_coro.clear();
        activateWaiter();
    }
    else {

    }
}

