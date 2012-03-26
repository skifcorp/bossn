#ifndef PORTER_H
#define PORTER_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QList>
#include <QTimer>
#include <QEvent>


#include "iodevicewrapper.h"
#include "scheduler.h"
#include "porterdriver.h"

struct TagMethod
{
    QString tag_name;
    QString method_name;
    TagMethod(const QString& tn, const QString & mn):tag_name(tn), method_name(mn){}
    TagMethod(){}
};

class Porter : public QObject
{
    Q_OBJECT    
public:
    Porter(const Porter& ) = delete;
    typedef QSharedPointer<Porter> Pointer;
    Porter(bool s):scheduled(s)
    {
    }

    ~Porter()
    {
    }

    void setScheduled(bool s);
    bool isScheduled() const {return scheduled;}

    void setDevice(const QString& n, const QMap<QString, QVariant> & settings );
    void addDriver(const QString&, const QMap<QString, QVariant>& drv_conf, const QList<TagMethod>& );


    //template <class... Args>
    QVariant value(const QString& n, QGenericArgument val0 = QGenericArgument( 0 ), QGenericArgument val1 = QGenericArgument( 0 )) const;
protected:

private:    
    typedef QList<PorterDriver::Pointer> Drivers;
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

    IoDeviceWrapper::Pointer device;
    Scheduler scheduler;
    bool scheduled;

    void addTagToSchedule(Drivers::size_type, const QString& tag_name );
};



#endif // PORTER_H
