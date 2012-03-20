#ifndef WEIGHTER_H
#define WEIGHTER_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QList>
#include <QTimer>
#include <QEvent>

#include "weight.h"
#include "iodevicewrapper.h"
#include "scheduler.h"



class Weighter : public QObject
{
    Q_OBJECT    
public:
    struct DriverContext
    {
        DriverContext(WeightDriver::Pointer d) : driver(d), value(0.0f), error(0){}
        WeightDriver::Pointer driver;
        float value;
        uint error;
    };

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
    void addDriver(const QString&, const QMap<QString, QVariant>& drv_conf );

    float weight(QList<DriverContext>::size_type idx) const;
protected:

private:    
    QList<DriverContext> drivers;
    IoDeviceWrapper::Pointer weight_device;
    Scheduler scheduler;
    bool scheduled;

    void addDriverToSchedule(QList<DriverContext>::size_type );
};



#endif // WEIGHTER_H
