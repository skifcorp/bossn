#ifndef DISPLAYWINSTAR_H
#define DISPLAYWINSTAR_H

#include "porterdriver.h"
#include <QVariant>
#include <QTextCodec>

class DisplayWinstar : public PorterDriver
{
    Q_OBJECT
public:
    static PorterDriver * create(const QVariantMap& p)
    {
        return new DisplayWinstar(p);
    }

    Q_INVOKABLE QVariant printText(const QVariant&);

protected:
    DisplayWinstar(const QVariantMap& );

private:
    static BossnFactoryRegistrator<DisplayWinstar>  registator;
    static uchar frame_ident;

    int RusToDisplay(int symv);
};

#endif // DISPLAYWINSTAR_H
