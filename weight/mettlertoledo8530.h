#ifndef __METTLER_TOLEDO_8530_H__
#define __METTLER_TOLEDO_8530_H__

#include "porterdriver.h"

class MettlerToledo8530 : public PorterDriver
{
    Q_OBJECT
public:
    //~MettlerToledo8530 () {}
    Q_INVOKABLE void readWeight(QVariant & ret, uint &);
    void zero(uint &);


    static MettlerToledo8530 * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new MettlerToledo8530(drv_conf);
    }
protected:
    MettlerToledo8530(const QMap<QString, QVariant>& conf):address(0)
    {
        bool ok = false;
        address = static_cast<uchar> ( conf["address"].toUInt(&ok) ); Q_ASSERT(ok) ;
    }

    /*static bool registerInFact()
    {
        factory_map().insert("MettlerToledo8530", &MettlerToledo8530::create);
        return true;
    }*/

    virtual QByteArray weightRequestFrame() const;
    virtual float parseWeightFrameAnswer(const QByteArray& ba, uint &, const QByteArray& cmd) const;

    virtual QByteArray zeroRequestFrame(uchar) const;
    virtual void  parseZeroFrameAnswer(const QByteArray&, uint & ) const;

    uchar address;
    //static bool registered;
    static BossnFactoryRegistrator<MettlerToledo8530> registrator;
};

#endif
