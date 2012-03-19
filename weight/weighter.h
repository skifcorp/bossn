#ifndef WEIGHTER_H
#define WEIGHTER_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QList>
#include <QTimer>
#include <QEvent>

#include "weight.h"
#include "qextserialport.h"
#include "weightdevice.h"

//#include "coroutine.h"

class Coroutine;

class Weighter : public QObject
{
    Q_OBJECT    
public:
    typedef QList<float> Values;
    Weighter(const Weighter& ) = delete;
    typedef QSharedPointer<Weighter> Pointer;

    Weighter():current_driver(0),error_timer(this)
    {
        error_timer.setInterval(500);
        error_timer.setSingleShot(true);

        timer.setInterval(500);
        timer.setSingleShot(true);

        connect(&error_timer, SIGNAL(timeout()), this, SLOT(onErrorTimer()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(onTimer()));

//        connect(weight_device.data(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    }
    ~Weighter()
    {
        //serial_port.close();
    }

/*    void setSettings(const PortSettings& s)
    {
        serial_port.setBaudRate(s.BaudRate);
        serial_port.setDataBits(s.DataBits);
        serial_port.setStopBits(s.StopBits);
        serial_port.setParity(s.Parity);
        serial_port.setFlowControl(s.FlowControl);
        serial_port.setTimeout( s.Timeout_Millisec );
    }

    void setPortName(const QString& n)
    {
        serial_port.setPortName(n);
    }*/

    void setSettings( const QMap<QString, QVariant> & s)
    {
        weight_device->setSettings(s);
    }

    void setWeightDevice(const QString& n, const QMap<QString, QVariant> & settings)
    {
        weight_device = WeightDevice::create(n);
        connect(weight_device.data(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));

        setSettings(settings);

    }

    void addDriver(const QString&, const QMap<QString, QVariant>& drv_conf );

    float weight(Values::size_type idx) const
    {
        return values[idx];
    }
private slots:
    void onReadyRead();
    void onErrorTimer();
    void onTimer();
private:    
    void readWeight();
    void incCurrent()
    {
        ++current_driver;
        if (current_driver >=drivers.size() )
            current_driver = 0;
    }
    void execute();
    typedef QList<QSharedPointer<WeightDriver> > WeightDrivers;
    typedef QSharedPointer<Coroutine> CoroutinePointer;
    typedef QList<uint> Errors;

    //QextSerialPort serial_port;
    WeightDevice::Pointer weight_device;

    WeightDrivers drivers;
    Values        values;
    Errors        errors;

    WeightDrivers::size_type current_driver;
    CoroutinePointer current_coro;

    QTimer error_timer;
    QTimer timer;
};



#endif // WEIGHTER_H
