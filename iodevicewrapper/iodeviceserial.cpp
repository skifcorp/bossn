#include "iodeviceserial.h"

BossnFactoryRegistrator<IoDeviceSerial> IoDeviceSerial::registrator("IoDeviceSerial");


void IoDeviceSerial::setSettings(const QMap<QString, QVariant>& s)
{
    {
        auto iter = s.find("portName");
        if (iter != s.end()) serial_port.setPortName(iter->toString());
    }
    {
        auto iter = s.find("baudRate");
        if (iter != s.end()) serial_port.setBaudRate(static_cast<BaudRateType>(iter->toUInt()));
    }
    {
        auto iter = s.find("dataBits");
        if (iter != s.end()) serial_port.setDataBits(static_cast<DataBitsType>(iter->toUInt()));
    }
    {
        auto iter = s.find("stopBits");
        if (iter != s.end()) serial_port.setStopBits(static_cast<StopBitsType>(iter->toUInt()));
    }
    {
        auto iter = s.find("parity");
        if (iter != s.end()) serial_port.setParity(static_cast<ParityType>(iter->toUInt()));
    }
    {
        auto iter = s.find("flowControl");
        if (iter != s.end()) serial_port.setFlowControl(static_cast<FlowType>(iter->toUInt()));
    }
    {
        auto iter = s.find("timeout");
        if (iter != s.end()) serial_port.setTimeout(iter->toUInt());
    }
}
