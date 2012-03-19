#include "weighter.h"
#include "weight.h"

#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>

#include <functional>

using std::bind;

void Weighter::addDriver(const QString & n, const QMap<QString, QVariant>& drv_conf)
{    
    WeightDriver::Pointer d = WeightDriver::create(n, drv_conf); Q_ASSERT(!d.isNull());

    if (drivers.isEmpty()) {
        qDebug () << "openening port: " << weight_device->deviceName();
        if ( !weight_device->open(QIODevice::ReadWrite ) ) {
            qWarning() << "cant open device!!!!: " <<weight_device->deviceName();
        }
        else {
            timer.start();
        }
    }

    drivers.append(d);
    values.append(0.0f);
    errors.append(0);
}

void Weighter::onTimer()
{
    error_timer.start();

    current_coro = QSharedPointer<Coroutine>(
                Coroutine::build( bind(&WeightDriver::readWeight, drivers[current_driver].data(),
                                       weight_device.data(),
                                       std::ref(values[current_driver]),
                                       std::ref(errors[current_driver])) ) );

    execute();
}


void Weighter::execute()
{
    errors[current_driver] = 0;
    current_coro->cont();
    Coroutine::Status s = current_coro->status();

    if (errors[current_driver])  {
        qWarning() << "ERROR!!! s: "<<s<< " error: "<<errors[current_driver];
        current_coro.clear();
    }

    if (s == Coroutine::NotStarted || s == Coroutine::Terminated ) {
        qDebug () << QDateTime::currentDateTime() <<" weight: " <<values[current_driver];
        incCurrent();
        error_timer.stop();
        timer.start();
    }
}

void Weighter::onReadyRead()
{   
    execute();
}

void Weighter::onErrorTimer()
{
    values[current_driver] = NAN;
    //serial_port.reset();
    weight_device->clear();
    incCurrent();
    timer.start();
    qDebug () << "not answered!!! port: "<<weight_device->deviceName();
}


