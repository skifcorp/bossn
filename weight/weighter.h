#ifndef WEIGHTER_H
#define WEIGHTER_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QList>
#include <QTimer>
#include <QEvent>

#include "weight.h"
#include "iodevicewrapper.h"
#include "scheduler.h"


struct TagMethod
{
    QString tag_name;
    QString method_name;
    TagMethod(const QString& tn, const QString & mn):tag_name(tn), method_name(mn){}
    TagMethod(){}
};

class Weighter : public QObject
{
    Q_OBJECT    
public:
    /*struct DriverContext
    {
        DriverContext(WeightDriver::Pointer d) : driver(d), error(0){}
        WeightDriver::Pointer driver;
        //QVariant value;
        //uint error;
        //QString method;
    };*/

    Weighter(const Weighter& ) = delete;
    typedef QSharedPointer<Weighter> Pointer;
    Weighter(bool s):scheduled(s)
    {
    }

    ~Weighter()
    {
    }

    void setScheduled(bool s);
    bool isScheduled() const {return scheduled;}

    void setWeightDevice(const QString& n, const QMap<QString, QVariant> & settings);
    void addDriver(const QString&, const QMap<QString, QVariant>& drv_conf, const QList<TagMethod>& );


    template <class... Args>
    QVariant value(const QString& n, Args... args) const
    {

        if (scheduled) {
            //return drivers[methods[n].driver_idx].value;

        }

        MethodInfo mi = methods[n];

        QVariant ret;
        QMetaObject::invokeMethod( drivers[mi.driver_idx].data(), mi.method.toAscii().data(),
                                        Q_RETURN_ARG(QVariant, ret),  Q_ARG(decltype(args), args)... );

        return ret;
    }


protected:

private:    
    //typedef QList<DriverContext> DriverContexts;
    typedef QList<WeightDriver::Pointer> Drivers;
    struct MethodInfo
    {
        QString method;
        Drivers::size_type driver_idx;
        QVariant value;
        uint error;

        MethodInfo(const QString& m, Drivers::size_type idx ) : method(m), driver_idx(idx), error(0) {}
        MethodInfo():driver_idx(0),error(0){}
    };

    Drivers                        drivers;
    QMap<QString, MethodInfo>      methods;

    IoDeviceWrapper::Pointer weight_device;
    Scheduler scheduler;
    bool scheduled;

    void addTagToSchedule(Drivers::size_type, const QString& tag_name );
};



#endif // WEIGHTER_H
