#ifndef __METTLER_TOLEDO_8530_H__
#define __METTLER_TOLEDO_8530_H__

#include "weight.h"

class MettlerToledo8530 : public WeightDriver
{
public:
    //~MettlerToledo8530 () {}
    virtual void readWeight(WeightDevice *, float & ret, uint &);
    virtual void zero(WeightDevice *, uint &);


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

    static bool registerInFact()
    {
        factory_map().insert("MettlerToledo8530", &MettlerToledo8530::create);
        return true;
    }

    virtual QByteArray weightRequestFrame() const;
    virtual float parseWeightFrameAnswer(const QByteArray& ba, uint &) const;

    virtual QByteArray zeroRequestFrame(uchar) const;
    virtual void  parseZeroFrameAnswer(const QByteArray&, uint & ) const;

    uchar address;
    static bool registered;
};

#endif
