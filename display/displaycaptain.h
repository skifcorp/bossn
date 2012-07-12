#ifndef DISPLAYCAPTAIN_H
#define DISPLAYCAPTAIN_H

#include "porterdriver.h"
#include <QVariant>
#include "mifarereader.h"

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
    MifareRequestFrame printTextFrame(const QString& ) const;
    MifareRequestFrame scrollTextFrame() const;

    bool processAnswer(const MifareRequestFrame& req_frame);
    static const QString end_of_str;
};

#endif
