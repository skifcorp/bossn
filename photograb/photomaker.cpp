#include "photomaker.h"
#include <QDebug>

static struct PhotoLib
{
   QLibrary lib;
   void (*grab)(const wchar_t*, const int);

   PhotoLib() : lib("IVC100DLL"),
       grab((void (*)(const wchar_t*,const int))lib.resolve("GrabFrame"))
   {
       ((int (*)(int))lib.resolve("Connect"))(1);
   }

   ~PhotoLib()
   {
       ((int (*)(int))lib.resolve("Disconnect"))(1);
       lib.unload();
   }

} photoLib;


void grabPhoto(const wchar_t* name, int i)
{
    photoLib.grab(name, i);
}
