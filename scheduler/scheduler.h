#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QTimer>

#include <functional>

using std::function;

#include "coroutine.h"


class IoDeviceWrapper;

class Scheduler : public QObject
{
    Q_OBJECT
public:
    typedef QWeakPointer<IoDeviceWrapper> IoDevPointer;

    Scheduler ();
    ~Scheduler(){}

    void setErrorPeriod(int msec)
    {
        error_timer.setInterval(msec);
    }
    void setSchedulePeriod(int msec)
    {
        schedule_timer.setInterval(msec);
    }

    void addFunction( function<void ()>, function<void ()>  );
    void setDevice(IoDevPointer d);
    void clear();
private slots:
    void onErrorTimer();
    void onScheduleTimer();
    void onReadyRead();
private:
    typedef QSharedPointer<Coroutine> CoroPointer;
    typedef QList<function<void ()> > Functions;

    Functions schedule_functions;
    Functions timeout_functions;
    Functions::size_type current_func;

    CoroPointer        current_coro;
    IoDevPointer       device;

    QTimer             error_timer;
    QTimer             schedule_timer;

    void incCurrent()
    {
        ++current_func;
        if (current_func >=schedule_functions.size() )
            current_func = 0;
    }
    void execute();

};

#endif // SCHEDULER_H
