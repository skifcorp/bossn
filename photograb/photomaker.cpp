#include "photomaker.h"
#include <QDebug>

CaptureLib::CaptureLib() : lib("IVC100DLL"),
  play((int (*)(const wchar_t*,const int))lib.resolve("GrabFrame"))
 {
    ((int (*)(int))lib.resolve("Connect"))(1);
 }

CaptureLib::~CaptureLib()
{
    ((int (*)(int))lib.resolve("Disconnect"))(1);
    lib.unload();
}


CaptureLib::Pointer CapturePhoto::lib;







