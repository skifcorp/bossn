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

    drivers.append( DriverContext(d) );
    if ( scheduled ) {
        addDriverToSchedule( drivers.count() - 1 );
    }
}

void Weighter::setWeightDevice(const QString& n, const QMap<QString, QVariant> & settings)
{
    weight_device = IoDeviceWrapper::create(n);

    weight_device->setSettings(settings);
    qDebug () << "openening port: " << weight_device->deviceName();
    if  (!weight_device->open(QIODevice::ReadWrite) ) {
        qWarning() << "cant open device!!!!: " <<weight_device->deviceName();
    }

    scheduler.setDevice(weight_device);
}

void Weighter::setScheduled(bool s)
{
    if (s && !scheduled) {
        for (auto i = 0; i<drivers.size(); ++i) {
            addDriverToSchedule(i);
        }
    }
    else if (scheduled && !s) {
        scheduler.clear();
    }

    scheduled = s;
}

void Weighter::addDriverToSchedule(QList<DriverContext>::size_type driver_index)
{
    scheduler.addFunction(
            [&weight_device, &drivers, driver_index] {
                drivers[driver_index].driver->readWeight(weight_device.data(), drivers[driver_index].value,  drivers[driver_index].error);
            },
            [&weight_device, &drivers, driver_index] {
                drivers[driver_index].value = NAN; drivers[driver_index].error = WeightDriver::WeightFrameNotAnswer;
                qDebug () << weight_device->deviceName() << " dont answered!";
            });
}

float Weighter::weight(QList<DriverContext>::size_type idx) const
{
    if (scheduled)
        return drivers[idx].value;

    float ret;
    uint error;

    drivers[idx].driver->readWeight(weight_device.data(), ret, error);
    if (error) return NAN;

    return ret;
}

