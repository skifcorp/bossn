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
#include "tagmethod.h"


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
    void addDriver(const QString&, const QMap<QString, QVariant>& drv_conf, const QList<TagMethod<false>>& );

    Q_INVOKABLE QVariant value (const QString& n, AlhoSequence *,
                   QGenericArgument val0 = QGenericArgument(),
                   QGenericArgument val1 = QGenericArgument(),
                   QGenericArgument val2 = QGenericArgument(),
                   QGenericArgument val3 = QGenericArgument(),
                   QGenericArgument val4 = QGenericArgument(),
                   QGenericArgument val5 = QGenericArgument(),
                   QGenericArgument val6 = QGenericArgument(),
                   QGenericArgument val7 = QGenericArgument(),
                   QGenericArgument val8 = QGenericArgument() );

    Q_INVOKABLE QVariant exec(const QString& n, AlhoSequence *,
                               QGenericArgument func_name,
                              QGenericArgument val1 = QGenericArgument(),
                              QGenericArgument val2 = QGenericArgument(),
                              QGenericArgument val3 = QGenericArgument(),
                              QGenericArgument val4 = QGenericArgument(),
                              QGenericArgument val5 = QGenericArgument(),
                              QGenericArgument val6 = QGenericArgument(),
                              QGenericArgument val7 = QGenericArgument(),
                              QGenericArgument val8 = QGenericArgument());
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
