#ifndef WEIGHTDEVICESERIAL_H
#define WEIGHTDEVICESERIAL_H

#include "iodevicewrapper.h"
#include "qextserialport.h"

class IoDeviceSerial : public IoDeviceWrapper
{
public:
    ~IoDeviceSerial() {}

    virtual void setSettings(const QMap<QString, QVariant>& );
    virtual QString deviceName() const
    {
        return serial_port.portName();
    }
    virtual void clear()
    {
        serial_port.reset();
    }

    static IoDeviceWrapper * create()
    {
        return new IoDeviceSerial();
    }

protected:
    IoDeviceSerial(){}
private:
    QextSerialPort serial_port;
    static bool registered;
    static bool registerInFact()
    {
        factory_map().insert("IoDeviceSerial", &IoDeviceSerial::create);
        return true;
    }

    virtual QIODevice * internalGetDevice() { return &serial_port; }
    virtual const QIODevice * internalGetDevice() const { return &serial_port; }
};

#endif // WEIGHTDEVICESERIAL_H
