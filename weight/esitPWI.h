#ifndef __QT_ESIT_PWI_H__
#define __QT_ESIT_PWI_H__

#include "weight.h"

class EsitPWI : public WeightDriver
{
public:
    virtual void readWeight(WeightDevice *, float & ret, uint &);
    virtual void zero(WeightDevice *, uint &);

    static EsitPWI * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new EsitPWI(drv_conf);
    }

protected:
    EsitPWI(const QMap<QString, QVariant>& conf) : WeightDriver()
    {
        bool ok = false;
        mode    = conf["mode"].toInt(&ok); Q_ASSERT(ok);
        address = static_cast<uchar> ( conf["address"].toUInt(&ok) ); Q_ASSERT(ok) ;
    }
private:
    int mode;
    QByteArray weightRequestFrameM1() const;
    QByteArray weightRequestFrameM3() const;

    float parseWeightFrameAnswerM1(const QByteArray&, uint & ) const;
    float parseWeightFrameAnswerM3(const QByteArray&, uint & ) const;

    uchar address;

    static bool registerInFact()
    {
        factory_map().insert("EsitPWI", &EsitPWI::create);
        return true;
    }

    QByteArray weightRequestFrame(uint & err) const
    {
        if (mode == 1 ) return weightRequestFrameM1();
        else if (mode == 3) return weightRequestFrameM3();

        err = WeightFrameBadConf;

        return QByteArray();
    }

    float parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
    {
        if (mode == 1 ) return parseWeightFrameAnswerM1(ba, err);
        else if (mode == 3) return parseWeightFrameAnswerM3(ba, err);

        //throw WeightFrameExceptionBadConf();
        err = WeightFrameBadConf;

        return NAN;
    }

    static bool registered;
};

#endif
