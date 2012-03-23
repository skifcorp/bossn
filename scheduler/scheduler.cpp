#include <QDebug>

#include "scheduler.h"
#include "iodevicewrapper.h"
#include "coroutine.h"
#include <QCoreApplication>

Scheduler::Scheduler ()
{

}

void Scheduler::setDevice(IoDevPointer d)
{
    if (!device.isNull()) {
        clear();
    }
    device = d;
    connect(device.toStrongRef().data(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}


void Scheduler::clear()
{
    if (!device.isNull())
        device.toStrongRef().data()->disconnect(SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    current_coro.clear();
    scheduls.clear();
    conn_obj.clear();
}

void Scheduler::addFunction(function<void ()>schf, function<void ()> tmf, int sch_msec, int tm_msec)
{
    scheduls.push_back(Schedul(schf, tmf, sch_msec, tm_msec));
    Schedul & s = scheduls.back();
    s.num = scheduls.count() - 1;
    connect_callable(s.schedule_timer.data(), SIGNAL(timeout()), this, bind(&Scheduler::onScheduleTimer, this, std::ref(s) ));
    connect(s.timeout_timer.data(), SIGNAL(timeout()), this, SLOT(onTimeoutTimer()));

    s.schedule_timer->start();
}

void Scheduler::onTimeoutTimer()
{    
    current_coro.schedul->timeout_func();
    current_coro.schedul->schedule_timer->start();

    current_coro.clear();
}

void Scheduler::onScheduleTimer(Schedul & s)
{   
    while (busy()) {
        qApp->processEvents();
    }

    qDebug() << "coro: "<<s.num;

    current_coro = CoroContext(QSharedPointer<Coroutine>(  Coroutine::build( s.schedule_func ) ), &s);

    execute();
}

void Scheduler::onReadyRead()
{
    execute();
}

void Scheduler::execute()
{
    current_coro.schedul->timeout_timer->start();

    current_coro.coro->cont();

    Coroutine::Status s = current_coro.coro->status();

    if (s == Coroutine::NotStarted || s == Coroutine::Terminated ) {
        current_coro.schedul->timeout_timer->stop();
        current_coro.schedul->schedule_timer->start();
        current_coro.clear();
    }
}
