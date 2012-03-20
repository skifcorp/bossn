#include <QDebug>

#include "scheduler.h"
#include "iodevicewrapper.h"
#include "coroutine.h"


Scheduler::Scheduler () : current_func(0)
{
    error_timer.setInterval(500);
    error_timer.setSingleShot(true);

    schedule_timer.setInterval(500);
    schedule_timer.setSingleShot(true);

    connect(&error_timer   , SIGNAL(timeout()), this, SLOT(onErrorTimer()));
    connect(&schedule_timer, SIGNAL(timeout()), this, SLOT(onScheduleTimer()));
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
    device.toStrongRef().data()->disconnect(SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    current_coro.clear();
    schedule_functions.clear();
    timeout_functions.clear();
    current_func = 0;

    schedule_timer.stop();
    error_timer.stop();
}

void Scheduler::addFunction(function<void ()>schedule_func, function<void ()> timeout_func)
{
    if (schedule_functions.empty()) {
        schedule_timer.start();
    }

    schedule_functions.append(schedule_func);
    timeout_functions.append(timeout_func);
}

void Scheduler::onErrorTimer()
{
    current_coro.clear();
    timeout_functions[current_func]();
    incCurrent();
    schedule_timer.start();
}

void Scheduler::onScheduleTimer()
{
    if (schedule_functions.empty()) return;

    error_timer.start();
    current_coro = QSharedPointer<Coroutine>(  Coroutine::build( schedule_functions[current_func] ) );

    execute();
}

void Scheduler::onReadyRead()
{
    error_timer.start();
    execute();
}

void Scheduler::execute()
{
    current_coro->cont();
    Coroutine::Status s = current_coro->status();

    if (s == Coroutine::NotStarted || s == Coroutine::Terminated ) {
        current_coro.clear();
        incCurrent();
        error_timer.stop();
        schedule_timer.start();
    }
}
