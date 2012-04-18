#ifndef DISPLAYFUTABA_H
#define DISPLAYFUTABA_H

#include "porterdriver.h"
#include <QVariant>
#include <QTextCodec>

class DisplayFutaba : public PorterDriver
{
    Q_OBJECT
public:
    static PorterDriver * create(const QVariantMap& p)
    {
        return new DisplayFutaba(p);
    }

    Q_INVOKABLE QVariant printText(const QVariant&);

protected:
    DisplayFutaba(const QVariantMap& );

private:
    static BossnFactoryRegistrator<DisplayFutaba>  registator;
    //uchar address;
    static uchar frame_ident;

    int RusToFutaba(QChar s);

};

#endif // DISPLAYFUTABA_H
