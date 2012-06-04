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
        device.toStrongRef().data()->disconnect(SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        clear();
    }
    device = d;
    connect(device.toStrongRef().data(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    //qDebug() << "connect!!!";
}


void Scheduler::clear()
{
    //if (!device.isNull()) {
        //device.toStrongRef().data()->disconnect(SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        //qDebug() << "disconnect!!!";
    //}
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

    //qDebug() << "timer Started!!!!";
}

void Scheduler::execFunction(function<void ()>schf, function<void ()> tmf, int tm_msec)
{
   // qDebug () << "exec function!";

    waitForFree();
    Schedul s(schf, tmf, 0, tm_msec);
    connect(s.timeout_timer.data(), SIGNAL(timeout()), this, SLOT(onTimeoutTimer()));
    startNewCoro(s);

    waitForFree();

    //qDebug() << "exit from exec function!";
}

void Scheduler::onTimeoutTimer()
{    
    qDebug() << "onTimeout timer!";

    current_coro.schedul->timeout_func();
    device.data()->clear();

    current_coro.schedul->schedule_timer->start();
    current_coro.clear();
}

void Scheduler::startNewCoro(Schedul & s)
{
    current_coro = CoroContext(QSharedPointer<Coroutine>(  Coroutine::build( s.schedule_func ) ), &s);

    execute();
}

void Scheduler::onScheduleTimer(Schedul & s)
{   
    //qDebug () << "on schedule timer: { "<<device.data()->deviceName();

    waitForFree();

    startNewCoro(s);

    //qDebug() << "}";
}

void Scheduler::onReadyRead()
{
    //qDebug() << "onReadY read!!!";
    execute();
}

void Scheduler::execute()
{
    //qDebug() << "execute started! {";

    if (!current_coro.schedul) {
        qWarning() << "\n\n\nsomething terrible! GOT EVENT ON PORT WHILE NOTHING WRITED!!\n\n\n"<<device.data()->deviceName();
        //device.data()->clear();
        return;
    }

    //qDebug("1");

    current_coro.schedul->timeout_timer->start();

    //qDebug("2");

    current_coro.coro->cont();

    //qDebug("3");

    Coroutine::Status s = current_coro.coro->status();

    //qDebug("4");

    if (s == Coroutine::NotStarted || s == Coroutine::Terminated ) {
        //qDebug() << "WOW!!!!! SHIT!!!" << "coroutine state: " << s <<device.data()->deviceName();
        current_coro.schedul->timeout_timer->stop();
        current_coro.schedul->schedule_timer->start();
        current_coro.clear();
    }
    else {

    }
    //qDebug("}");
}
