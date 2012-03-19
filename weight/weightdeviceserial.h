#ifndef WEIGHTDEVICESERIAL_H
#define WEIGHTDEVICESERIAL_H

#include "weightdevice.h"
#include "qextserialport.h"

class WeightDeviceSerial : public WeightDevice
{
public:
    ~WeightDeviceSerial() {}

    virtual void setSettings(const QMap<QString, QVariant>& );
    virtual QString deviceName() const
    {
        return serial_port.portName();
    }
    virtual void clear()
    {
        serial_port.reset();
    }

    static WeightDevice * create()
    {
        return new WeightDeviceSerial();
    }

protected:
    WeightDeviceSerial(){}
private:
    QextSerialPort serial_port;
    static bool registered;
    static bool registerInFact()
    {
        factory_map().insert("WeightDeviceSerial", &WeightDeviceSerial::create);
        return true;
    }

    virtual QIODevice * internalGetDevice() { return &serial_port; }
    virtual const QIODevice * internalGetDevice() const { return &serial_port; }
};

#endif // WEIGHTDEVICESERIAL_H
