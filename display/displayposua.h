#ifndef DISPLAYPOSUA_H
#define DISPLAYPOSUA_H

#include "porterdriver.h"
#include <QVariant>
#include <QTextCodec>

class DisplayPosua : public PorterDriver
{
    Q_OBJECT
public:
    static PorterDriver * create(const QVariantMap& p)
    {
        return new DisplayPosua(p);
    }

    Q_INVOKABLE QVariant printText(const QVariant&);

protected:
    DisplayPosua(const QVariantMap& );

private:
    static BossnFactoryRegistrator<DisplayPosua>  registator;
    //uchar address;
    static uchar frame_ident;

    int RusToDisplay(QChar s);

};

#endif // DISPLAYPOSUA_H
