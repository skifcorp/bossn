#include "iodevicediscrete.h"

bool IoDeviceDiscrete::registered = IoDeviceDiscrete::registerInFact();

#ifdef Q_OS_WIN32

#include "wdt.h"

DioDevice::DioDevice()
{
}

DioDevice::~DioDevice()
{
    close();
}

bool DioDevice::open(QIODevice::OpenModeFlag )
{
    wchar_t s[device_name.length() + 1];
    int len = device_name.toWCharArray(s);
    s[len] = '\n';

    handle = CreateFile(s, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    return handle!=INVALID_HANDLE_VALUE;
}

void DioDevice::close()
{
    CloseHandle(handle);
}


qint64 DioDevice::readData ( char * data, qint64 maxSize )
{
    if  (maxSize < 1) return 0ul;

    WDTPARAM cParam;
    DWORD nReturn;

    BOOL ret = DeviceIoControl(handle, IOCTL_SYS_DIO_READ, &cParam, sizeof(WDTPARAM), &cParam, sizeof(WDTPARAM), &nReturn, NULL);

    if (!ret) {
        qWarning("Cant read dido!!!!"); return 0ul;
    }

    data[0]=cParam.timeout;

    return 1ul;
}

qint64	DioDevice::writeData ( const char * data, qint64 maxSize )
{
    if (maxSize < 1) return 0;

    WDTPARAM cParam;
    DWORD nReturn;

    cParam.data_b = data[0];
    BOOL ret = DeviceIoControl(handle, IOCTL_SYS_DIO_WRITE, &cParam, sizeof(WDTPARAM), 0, 0, &nReturn, NULL);
    if (!ret) {
        qWarning("Cant write dido!!!!"); return 0ul;
    }

    return 1ul;
}

#endif
