#ifndef WEIGHTDEVICESERIAL_H
#define WEIGHTDEVICESERIAL_H

#include <QMetaEnum>

#include "iodevicewrapper.h"
#include "qextserialport.h"
#include "factory.h"

class IoDeviceSerial : public IoDeviceWrapper
{
    Q_OBJECT
    /*Q_ENUMS(QextSerialPort::BaudRateType)
    Q_ENUMS(QextSerialPort::DataBitsType)
    Q_ENUMS(QextSerialPort::ParityType)
    Q_ENUMS(QextSerialPort::StopBitsType)
    Q_ENUMS(QextSerialPort::FlowType)*/
public:
    ~IoDeviceSerial() {}

    virtual void setSettings(const QMap<QString, QVariant>& );
    virtual QString deviceName() const
    {
        return serial_port.portName();
    }
    virtual void clear()
    {
        //if (!isDeviceOpened() ) return;
        //serial_port.reset();
        //serial_port.flush();
        //qDebug() << "start clearing COMPORT!!!!!!";
        QByteArray ba = readAll();
        Q_UNUSED(ba);
        //qDebug() << ba.size() << " bytes cleared in port: " << deviceName();
        /*auto m = serial_port.openMode();
        serial_port.flush();
        //qDebug() << "comport 1";
        serial_port.close();
        //qDebug() << "comport 2";
        serial_port.open(m);
        //qDebug() << "comport 3";
        serial_port.flush();*/
    }

    static IoDeviceWrapper * create()
    {
        return new IoDeviceSerial();
    }

protected:
    IoDeviceSerial(){}
private:
    QextSerialPort serial_port;
/*    static bool registered;
    static bool registerInFact()
    {
        factory_map().insert("IoDeviceSerial", &IoDeviceSerial::create);
        return true;
    }*/

    virtual QIODevice * internalGetDevice() { return &serial_port; }
    virtual const QIODevice * internalGetDevice() const { return &serial_port; }

    static BossnFactoryRegistrator<IoDeviceSerial> registrator;

    template <class T>
    T getEnumerableValue(const QString& enum_name, const QString& value) const
    {
        int enum_id = serial_port.metaObject()->indexOfEnumerator( enum_name.toAscii() );
        if ( enum_id == -1 ) {
            qWarning() << "cant get enumerator index for enum: "<< enum_name<< " value: "<<value;
            qFatal("exit!!!");
        }
        int v = serial_port.metaObject()->enumerator( enum_id ).keyToValue( value.toAscii() );
        if (v == -1) {
            qWarning() << "cant get enumerator value index for enum: "<< enum_name<< " value: "<<value;
            qFatal("exit!!!");
        }
        return static_cast<T>(v);
    }
};

#endif // WEIGHTDEVICESERIAL_H
