#ifndef __WEIGHT_FROM_FILE_H__
#define __WEIGHT_FROM_FILE_H__

#include "porterdriver.h"

class WeightFromFile : public PorterDriver
{
    Q_OBJECT
public:
    virtual void readWeight(IoDeviceWrapper *, float & ret, uint &);
    virtual void zero(IoDeviceWrapper *, uint &);

    static WeightFromFile * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new WeightFromFile(drv_conf);
    }
protected:
    WeightFromFile(const QMap<QString, QVariant>& )
    {
    }
    static bool registerInFact()
    {
        factory_map().insert("WeightFromFile", &WeightFromFile::create);
        return true;
    }

    virtual QByteArray weightRequestFrame() const;
    virtual float parseWeightFrameAnswer(const QByteArray& ba, uint &) const;

    static bool registered;
};

#endif
