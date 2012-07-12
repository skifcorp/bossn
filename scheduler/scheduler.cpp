#include <QDebug>

#include "scheduler.h"
#include "iodevicewrapper.h"
#include "coroutine.h"
#include "alhosequence.h"

#include <QCoreApplication>


void CoroContext::clear()
{
    if (activate_on_finish && schedul->running()) {
        //qDebug() << "!!!!!!!!!!!!!GOT activate_on_finish!!!!: status: ";
        schedul->cont();
        //qDebug() << "ret:"<< ret;
    }
    //qDebug() << "cleared !!!!";
    schedul = nullptr;
    coro.clear();
}


void Schedul::coro_deleter(Coroutine *c)
{
    delete c;
}

void Schedul::coro_deleter_for_external_coro(Coroutine *)
{

    //does nothing
}

void Schedul::onScheduleTimer()
{
    //qDebug() << "Schedul::onTimer()";
    coro->cont();
}

/*void Schedul::onTimeoutTimer()
{
    cont();
}*/

void Schedul::onSchedulerFinished()
{
    //qDebug() << "Schedul::onFinished()";
    coro->cont();
}

void Schedul::startScheduleTimer()
{
    //qDebug() << "schedul::start()";

    schedule_timer.start();
    coro = TrickyCoroPointer (
         Coroutine::build( [this](){scheduler.startNewCoro(*this, false, false, true);} ), coro_deleter );
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
    //qDebug() << "scheduler set device finished";
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
    scheduls.push_back( Schedul::Pointer( new Schedul(*this, schf, tmf, sch_msec, tm_msec)));
    Schedul & s = *scheduls.back();
    s.num = scheduls.count() - 1;
    //connect_callable(s.schedule_timer.data(), SIGNAL(timeout()), this, bind(&Scheduler::onScheduleTimer, this, std::ref(s) ));
    connect(&s.timeout_timer, SIGNAL(timeout()), this, SLOT(onTimeoutTimer()));

    s.startScheduleTimer();

    //qDebug() << "addFunction for dev: " <<device.data()->deviceName();;
}

void Scheduler::waitForFree (Schedul& s, bool important)
{
    if (!busy()) {
        return;
    }


    //connect(this, SLOT(finished()), &s, SLOT(onSchedulerFinished()), Qt::QueuedConnection);

    if (important)
        waiters.push_front(&s);
    else
        waiters.push_back(&s);

    while (busy()) {
        //qDebug() << "falling to sleep again";

        s.yield();
        //qDebug() << "try check if sleep needed!!!";
    }

    //qDebug() << "URA! I dont sleep!!";

    waiters.removeAll(&s);
}


void Scheduler::activateWaiter()
{
    if (waiters.isEmpty()) return;

    QTimer::singleShot(0, waiters.first(), SLOT(onSchedulerFinished()) );
}




void Scheduler::execFunction(AlhoSequence * caller, function<void ()>schf, function<void ()> tmf, int tm_msec)
{
    //qDebug () << "1: exec function! caller: " << caller->objectName();



    Schedul s(*this, schf, tmf, 0, tm_msec);
    s.setExternalCoro(caller);
    connect(&s.timeout_timer, SIGNAL(timeout()), this, SLOT(onTimeoutTimer()));

    startNewCoro(s, true, true, false);

    //qDebug() << "2: !!!!!!!!! exec function: afterStartNewCoro!!!!: " << caller->objectName();

    if (current_coro.coro && current_coro.coro->status() == Coroutine::Stopped ) {
        //qDebug() << "before yield!!!!! " << caller->objectName();
        s.yield();  //we will return here when finished !!!
        //qDebug() << "after yyyyy "<<caller->objectName();
    }
    else {
        //qDebug() << "not running!!"<<caller->objectName();
    }

    //qDebug () << "3: !!!!!!!!!!! URAAA exec function! exit for : " << caller->objectName();
}


void Scheduler::onTimeoutTimer()
{    
    //qDebug() << "onTimeout timer! "<<device.data()->deviceName();

    current_coro.schedul->timeout_func();
    //qDebug() << "11: dev_data: before clear!! ";
    device.data()->clear();
    //qDebug() << "22 after clear!!!";

    if (current_coro.cyclic )
        current_coro.schedul->startScheduleTimer();
    //qDebug() << "33";
    current_coro.clear();

    activateWaiter();

    //qDebug() << "exit from onTimeoutTimer!";
}

void Scheduler::startNewCoro(Schedul & s, bool important, bool activate_on_finish, bool cyclic)
{
    waitForFree(s, important);

    current_coro = CoroContext(QSharedPointer<Coroutine>(  Coroutine::build( s.schedule_func ) ), &s);
    current_coro.activate_on_finish = activate_on_finish;
    current_coro.cyclic = cyclic;
    current_coro.coro->createStack(65535*4);

    //qDebug() << "start coro: " << device.data()->deviceName();

    execute();
}

/*void Scheduler::onScheduleTimer(Schedul & s)
{   
    qDebug () << "on schedule timer: { "<<device.data()->deviceName();

    waitForFree();

    startNewCoro(s);

    //qDebug() << "}";
}*/

void Scheduler::onReadyRead()
{
    //qDebug() << "onReadY read!!!";
    execute();
}

void Scheduler::execute()
{
    //qDebug() << "execute started! {";

    if (!current_coro.schedul) {
        qWarning() << "\n\n\nsomething terrible! GOT EVENT ON PORT WHILE NOTHING WRITED!! "<<device.data()->deviceName()<<"bytes num:"
                   << device.data()->bytesAvailable() << "\n\n\n";
        device.data()->clear();
        return;
    }

    //qDebug("1");

    current_coro.schedul->timeout_timer.start();

    //qDebug("2");
    //qDebug () << "execute!!!";
    current_coro.coro->cont();
    //qDebug() << "after";
    //qDebug("3");

    Coroutine::Status s = current_coro.coro->status();

    //qDebug() << "coro: status: " << s;

    //qDebug("4");

    if (s == Coroutine::NotStarted || s == Coroutine::Terminated ) {
        current_coro.schedul->timeout_timer.stop();
        if (current_coro.cyclic)
            current_coro.schedul->startScheduleTimer();
        current_coro.clear();
        activateWaiter();
    }
    else {

    }
    //qDebug("}");
}

/*
void Scheduler::waitForFree(Schedul& s)
{

}
*/
