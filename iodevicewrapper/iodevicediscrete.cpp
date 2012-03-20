#include "iodevicediscrete.h"

#ifdef Q_OS_WIN32

DioDevice::DioDevice()
{
}

DioDevice::~DioDevice()
{
}

bool DioDevice::open(QIODevice::OpenModeFlag )
{

}

void DioDevice::close()
{

}


qint64	DioDevice::readData ( char * data, qint64 maxSize )
{

}

qint64	DioDevice::writeData ( const char * data, qint64 maxSize )
{

}

#endif
