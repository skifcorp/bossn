#include "weighter.h"
#include "weight.h"

#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>

#include <functional>

using std::bind;

Q_DECLARE_METATYPE(IoDeviceWrapper::Pointer::Type*)
int __id = qRegisterMetaType<IoDeviceWrapper::Pointer::Type*>("IoDeviceWrapper::Pointer::Type *");

void Weighter::addDriver(const QString & n, const QMap<QString, QVariant>& drv_conf, const QList<TagMethod> &tags_methods)
{    
    WeightDriver::Pointer d = WeightDriver::create(n, drv_conf); Q_ASSERT(!d.isNull());    

    drivers.append( d );
    auto idx = drivers.size() - 1;


    for (QList<TagMethod>::const_iterator iter = tags_methods.begin(); iter != tags_methods.end(); ++iter) {
        methods.insert(iter->tag_name, MethodInfo(iter->method_name, idx));
        if ( scheduled ) {
            addTagToSchedule( idx, iter->tag_name );
        }
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
            //addTagToSchedule(i); !!!!
        }
    }
    else if (scheduled && !s) {
        scheduler.clear();
    }

    scheduled = s;
}

void Weighter::addTagToSchedule(Drivers::size_type driver_index, const QString& tag_name)
{
    scheduler.addFunction(
            [&weight_device, &drivers, driver_index, &methods, tag_name] {
                MethodInfo & mi = methods[tag_name];

                QMetaObject::invokeMethod(drivers[driver_index].data(), mi.method.toAscii().data(),
                                          Q_ARG(IoDeviceWrapper::Pointer::Type*, weight_device.data()),
                                          Q_ARG(QVariant&, mi.value), Q_ARG(uint&, mi.error));
            },
            [&weight_device, &drivers, driver_index, &methods, tag_name] {
                MethodInfo & mi = methods[tag_name];
                mi.value  = NAN; mi.error = WeightDriver::WeightFrameNotAnswer;
                qDebug () << weight_device->deviceName() << " dont answered!";
            }, 500, 500);
}

/*float Weighter::weight(QList<DriverContext>::size_type idx) const
{
    if (scheduled)
        return drivers[idx].value;

    float ret = 0.0f;
    uint error=0;

    drivers[idx].driver->readWeight(weight_device.data(), ret, error);
    if (error) return NAN;

    return ret;
}*/

