#include "porter.h"


#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>

#include <functional>

using std::bind;

//Q_DECLARE_METATYPE(IoDeviceWrapper::Pointer::Type*)
//int __id = qRegisterMetaType<IoDeviceWrapper::Pointer::Type*>("IoDeviceWrapper::Pointer::Type *");

void Porter::addDriver(const QString & n, const QMap<QString, QVariant>& drv_conf, const QList<TagMethod> &tags_methods)
{    
    PorterDriver::Pointer d = PorterDriver::create(n, drv_conf); Q_ASSERT(!d.isNull());
    d->setIoDevice(device);

    drivers.append( d );
    auto idx = drivers.size() - 1;

    for (QList<TagMethod>::const_iterator iter = tags_methods.begin(); iter != tags_methods.end(); ++iter) {
        methods.insert(iter->tag_name, MethodInfo(iter->read_method_name, idx));
        if ( scheduled ) {
            addTagToSchedule( idx, iter->tag_name );
        }
    }
}

void Porter::setDevice(const QString& n, const QMap<QString, QVariant> & settings)
{
    device = IoDeviceWrapper::create(n);

    device->setSettings(settings);
    qDebug () << "openening port: " << device->deviceName();
    if  (!device->open(QIODevice::ReadWrite) ) {
        qWarning() << "cant open device!!!!: " <<device->deviceName();
    }

    if (scheduled)
        scheduler.setDevice(device);
}

void Porter::setScheduled(bool s)
{
    if (s && !scheduled) {
        scheduler.setDevice(device);
        for (auto i = 0; i<drivers.size(); ++i) {
            //addTagToSchedule(i); !!!!
        }
    }
    else if (scheduled && !s) {
        scheduler.clear();
    }

    scheduled = s;
}

void Porter::addTagToSchedule(Drivers::size_type driver_index, const QString& tag_name)
{
    scheduler.addFunction(
            [&device, &drivers, driver_index, &methods, tag_name] {
                MethodInfo & mi = methods[tag_name];

                QMetaObject::invokeMethod(drivers[driver_index].data(), mi.method.toAscii().data(),
                                          //Q_ARG(IoDeviceWrapper::Pointer::Type*, device.data()),
                                          Q_ARG(QVariant&, mi.value), Q_ARG(uint&, mi.error));
            },
            [&device, &drivers, driver_index, &methods, tag_name] {
                MethodInfo & mi = methods[tag_name];
                mi.value  = NAN; mi.error = PorterDriver::WeightFrameNotAnswer;
                qDebug () << device->deviceName() << " dont answered!";
            }, 500, 500);
}

QVariant Porter::value (const QString& n,  QGenericArgument val0, QGenericArgument val1, QGenericArgument val2,
                        QGenericArgument val3, QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, QGenericArgument val7, QGenericArgument val8 ) const
{
    if (scheduled) {
        return methods[n].value;
    }

    MethodInfo mi = methods[n];

    QVariant ret(true);
    bool res = QMetaObject::invokeMethod( drivers[mi.driver_idx].data(), mi.method.toAscii().data(), Q_RETURN_ARG(QVariant, ret),
                                    val0, val1, val2, val3, val4, val5, val6, val7, val8 );

    if (!res) {
        qWarning()<<"cant invoke "<<mi.method;
    }
    //qDebug() << "after value: "<<ret << " m: "<< mi.method.toAscii().data()<<" res: "<<res;

    //QMetaObject::invokeMethod( drivers[mi.driver_idx].data(), mi.method.toAscii().data(),
      //                              Q_RETURN_ARG(QVariant, ret),  Q_ARG(decltype(args), args)... );

    return ret;
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

