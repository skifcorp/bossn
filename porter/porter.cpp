#include "porter.h"


#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>

#include <functional>

using std::bind;



void Porter::addDriver(const QString & n, const QMap<QString, QVariant>& drv_conf, const QList<TagMethod<false>> &tags_methods)
{    
    PorterDriver::Pointer d = PorterDriver::create(n, drv_conf); Q_ASSERT(!d.isNull());
    d->setIoDevice(device);

    drivers.append( d );
    auto idx = drivers.size() - 1;

    for (QList<TagMethod<false>>::const_iterator iter = tags_methods.begin(); iter != tags_methods.end(); ++iter) {
        methods.insert(iter->tag_name, MethodInfo(iter->method_name, idx));
        if ( scheduled ) {
            addTagToSchedule( idx, iter->tag_name );
        }
    }
}

void Porter::setDevice(const QString& n, const QMap<QString, QVariant> & settings)
{
    qDebug() << "setDevice: " << n;

    device = IoDeviceWrapper::create(n);

    device->setSettings(settings);
    //qDebug () << "opening port: " << device->deviceName();
    if  (!device->open(QIODevice::ReadWrite) ) {
        qWarning() << "cant open device!!!!: " <<device->deviceName();
    }
    else {
        qDebug() << "succesully opened: " << device->deviceName();
    }

    //if (scheduled)
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
            [this, driver_index, tag_name] {
                MethodInfo & mi = methods[tag_name];

                QMetaObject::invokeMethod(drivers[driver_index].data(), mi.method.toAscii().data(),
                                          Q_ARG(QVariant&, mi.value), Q_ARG(uint&, mi.error));
                if (mi.error) {
                    qWarning() << "got error in porter device!!!!: code: "<<mi.error << "device: "<<device->deviceName();
                }
            },
            [this,   driver_index, tag_name] {
                MethodInfo & mi = methods[tag_name];
                mi.value  = 0; mi.error = PorterDriver::PorterFrameNotAnswer;
                qDebug () << device->deviceName() << " dont answered!";
                device->clear();
            }, 500, 1000);
}

template <class Ret>
inline Ret generic_arg_cast(QGenericArgument arg)
{
    if ( qstrcmp( arg.name(), "QVariant" ) ) {
        qWarning() << "generic_arg_cast Bad argument name!!!" << arg.name();
        qFatal("Exit");
        return Ret();
    }

    QVariant var = *reinterpret_cast<QVariant *>(arg.data());

    if ( static_cast<int> (var.type()) != qMetaTypeId<Ret>() ) {
        qWarning() << "generic_arg_cast: passed argument is not of type you want to convert from QVariant !!!"<<
                      static_cast<int>(var.type());
        Q_ASSERT(0);
        return Ret();
    }

    return var.value<Ret>();
}

QVariant Porter::exec(const QString& tag_name,  AlhoSequence * caller, QGenericArgument func,
        QGenericArgument val0,  QGenericArgument val1,
        QGenericArgument val2,  QGenericArgument val3,
        QGenericArgument val4,  QGenericArgument val5,
        QGenericArgument val6,  QGenericArgument val7)
{
    QString func_name = generic_arg_cast<QString>(func);

    MethodInfo mi = methods[tag_name];

    QVariant ret;

    scheduler.execFunction(caller,
                [this, &mi, &ret, &val0, &val1, &val2, &val3, &val4, &val5, &val6, &val7, &func_name]{
                    bool res = QMetaObject::invokeMethod( drivers[mi.driver_idx].data(),
                                 func_name.toAscii().data(), Q_RETURN_ARG(QVariant, ret),
                                    val0, val1, val2, val3, val4, val5, val6, val7 );
                    if (!res) {
                        qWarning()<<"cant invoke "<<func_name<< " on "<<drivers[mi.driver_idx].data()->metaObject()->className()
                                 <<" with args: (" <<val0.name()<<"), val1("<<val1.name()<<")\n";
                    }

                },
                [this, &mi]{
                    qWarning()<<"device: "<<device->deviceName()<<" not answered!!!!";
                    mi.error = PorterDriver::PorterFrameNotAnswer;
                    device->clear();
                },
                500,
                tag_name,
                func_name  );


    if (mi.error)   return QVariant();

    return ret;
}

QVariant Porter::value (const QString& n,  AlhoSequence * caller,
                        QGenericArgument val0, QGenericArgument val1, QGenericArgument val2,
                        QGenericArgument val3, QGenericArgument val4, QGenericArgument val5, QGenericArgument val6,
                        QGenericArgument val7, QGenericArgument val8 )
{
    MethodInfo mi = methods[n];

    if (scheduled) {
        if ( mi.error )  return QVariant();

        return methods[n].value;
    }

    QVariant ret;

    scheduler.execFunction(caller,
                [this, &mi, &ret, &val0, &val1, &val2, &val3, &val4, &val5, &val6, &val7, &val8]{
                    bool res = QMetaObject::invokeMethod( drivers[mi.driver_idx].data(),
                                 mi.method.toAscii().data(), Q_RETURN_ARG(QVariant, ret),
                                    val0, val1, val2, val3, val4, val5, val6, val7, val8 );
                    if (!res) {
                        qWarning()<<"cant invoke "<<mi.method<< " on "<<drivers[mi.driver_idx].data()->metaObject()->className()
                                 <<" with args: (" <<val0.name()<<"), val1("<<val1.name()<<")\n";
                    }

                },
                [this, &mi]{
                    qWarning()<<"device: "<<device->deviceName()<<" not answered!!!!";
                    mi.error = PorterDriver::PorterFrameNotAnswer;
                    device->clear();
                },
                500,
                n,
                mi.method);


    if (mi.error)  return QVariant();

    return ret;
}


