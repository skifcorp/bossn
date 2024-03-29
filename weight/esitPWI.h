#ifndef __QT_ESIT_PWI_H__
#define __QT_ESIT_PWI_H__

#include "porterdriver.h"

class EsitPWI : public PorterDriver
{
    Q_OBJECT
public:
    void readWeight(float & ret, uint &);
    void zero( uint &);

    static EsitPWI * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new EsitPWI(drv_conf);
    }

protected:
    EsitPWI(const QMap<QString, QVariant>& conf) : PorterDriver()
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

    /*static bool registerInFact()
    {
        factory_map().insert("EsitPWI", &EsitPWI::create);
        return true;
    }*/

    QByteArray weightRequestFrame(uint & err) const
    {
        if (mode == 1 ) return weightRequestFrameM1();
        else if (mode == 3) return weightRequestFrameM3();

        err = PorterFrameBadConf;

        return QByteArray();
    }

    float parseWeightFrameAnswer(const QByteArray& ba, uint & err) const
    {
        if (mode == 1 ) return parseWeightFrameAnswerM1(ba, err);
        else if (mode == 3) return parseWeightFrameAnswerM3(ba, err);

        //throw WeightFrameExceptionBadConf();
        err = PorterFrameBadConf;

        return NAN;
    }

    //static bool registered;
    static BossnFactoryRegistrator<EsitPWI> registrator;
};

#endif
