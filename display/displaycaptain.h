#ifndef DISPLAYCAPTAIN_H
#define DISPLAYCAPTAIN_H

#include "porterdriver.h"
#include <QVariant>

class DisplayCaptain : public PorterDriver
{
    Q_OBJECT
public:
    static PorterDriver * create(const QVariantMap& p)
    {
        return new DisplayCaptain(p);
    }
    Q_INVOKABLE QVariant printText(const QVariant&);
protected:
    DisplayCaptain(const QVariantMap& );
private:
    static BossnFactoryRegistrator<DisplayCaptain>  registator;
    uchar address;
    static uchar frame_ident;

};

#endif
