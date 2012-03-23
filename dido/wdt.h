// Date : 8/8/2000
// Company : ICP Electronics Inc.
// Description : Define the IO control code of watch dog timer.

#ifndef __WDT_H__
#define __WDT_H__

// Begin conditional compilation of platform dependency
#if defined(WIN32)	// for NT, Win2000, 98 platform

#ifdef WDM
#include <devioctl.h>
#else
#include <winioctl.h>	// contains the definition of CTL_CODE for Window application
#endif

#define WDT_DEVICE			"\\\\.\\WDT_DEVICE"
#define WDT_DEVICE_NAME			L"\\Device\\WDT_DEVICE"
#define WDT_DOS_DEVICE_NAME		L"\\DosDevices\\WDT_DEVICE"

// Device type
#define WDT_TYPE			35001

#if defined(WINSYS)
// define device extension structure
typedef struct tagWDTDEVICEINFO {
	PDEVICE_OBJECT pDeviceObject;
} WDTDEVICEINFO, *PWDTDEVICEINFO;
#endif

//***************************************************************
//#define	POS_370		// just for POS 370 outport = 0x440
//***************************************************************

typedef struct tagWDTPARAM {
	unsigned char	timeout;
	unsigned char	data_b;
} WDTPARAM, *PWDTPARAM;

// The IOCTL function codes from 0x800 to 0xFFF are for customer use.
#define IOCTL_SYS_WDT_SET_TIMEOUT	CTL_CODE(WDT_TYPE, 0x800, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SYS_WDT_START		CTL_CODE(WDT_TYPE, 0x801, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SYS_WDT_STOP		CTL_CODE(WDT_TYPE, 0x802, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SYS_WDT_RESTART		CTL_CODE(WDT_TYPE, 0x803, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SYS_DIO_READ		CTL_CODE(WDT_TYPE, 0x804, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SYS_DIO_WRITE		CTL_CODE(WDT_TYPE, 0x805, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#elif defined(LINUX)	// Linux platform

// IO control code
#define IOCTL_WDT_MAGIC			'W'
#define IOCTL_WDT_SET_TIMEOUT		_IOR(IOCTL_WDT_MAGIC, 0, sizeof(unsigned char))
#define IOCTL_WDT_START			_IO(IOCTL_WDT_MAGIC, 1)
#define IOCTL_WDT_STOP			_IO(IOCTL_WDT_MAGIC, 2)
#define IOCTL_WDT_RESTART		_IO(IOCTL_WDT_MAGIC, 3)

#ifdef __KERNEL__
#define DEFAULT_TIME_TO_RESET	30
#endif 	// __KERNEL__

#endif

#endif	// __WDT_H__
