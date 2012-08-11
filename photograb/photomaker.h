#ifndef PHOTOMAKER_H
#define PHOTOMAKER_H
#include <QLibrary>
#include <QObject>
#include <QSharedPointer>

//void grabPhoto(const wchar_t*, int);



class CaptureLib
{
public:
  // void (*grab)(const wchar_t*, const int);
    CaptureLib();
    ~CaptureLib();

    QLibrary lib;
    typedef int (*MyDll_1)(int);
    typedef int (*MyDll_2)(const wchar_t*, int);
    typedef int (*MyDll_3)(int);

    MyDll_1 connect;
    MyDll_2 play;
    MyDll_3 disconnect;

    typedef QSharedPointer<CaptureLib> Pointer;

};


class CapturePhoto
{
public:
    CapturePhoto()  {}
    ~CapturePhoto()
    {

    }

    void grabPhoto(const wchar_t * fName, int ch)
    {
        if (!lib )
        {
            lib = CaptureLib::Pointer (new CaptureLib());
        }

        lib->play(fName, ch);
    }

private:
    static CaptureLib::Pointer lib;
};




#endif // PHOTOMAKER_H
