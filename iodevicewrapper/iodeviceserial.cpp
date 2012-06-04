#include "iodeviceserial.h"

BossnFactoryRegistrator<IoDeviceSerial> IoDeviceSerial::registrator("IoDeviceSerial");




void IoDeviceSerial::setSettings(const QMap<QString, QVariant>& s)
{
    //qDebug() << "enum_count: "<<serial_port.metaObject()->enumeratorCount();

    {
        auto iter = s.find("portName");
        if (iter != s.end()) serial_port.setPortName(iter->toString());
    }
    {
        auto iter = s.find("baudRate");
        //if (iter != s.end()) serial_port.setBaudRate(static_cast<BaudRateType>(iter->toUInt()));
        if (iter != s.end()) serial_port.setBaudRate( getEnumerableValue<QextSerialPort::BaudRateType>("BaudRateType", iter->toString()));
    }
    {
        auto iter = s.find("dataBits");
        if (iter != s.end()) serial_port.setDataBits(getEnumerableValue<QextSerialPort::DataBitsType>("DataBitsType", iter->toString()));
    }
    {
        auto iter = s.find("stopBits");
        if (iter != s.end()) serial_port.setStopBits(getEnumerableValue<QextSerialPort::StopBitsType>("StopBitsType", iter->toString()));
    }
    {
        auto iter = s.find("parity");
        if (iter != s.end()) serial_port.setParity(getEnumerableValue<QextSerialPort::ParityType>("ParityType", iter->toString()));
    }
    {
        auto iter = s.find("flowControl");
        if (iter != s.end()) serial_port.setFlowControl(getEnumerableValue<QextSerialPort::FlowType>("FlowType", iter->toString()));
    }
    {
        auto iter = s.find("timeout");
        if (iter != s.end()) serial_port.setTimeout(iter->toUInt());
    }
}
