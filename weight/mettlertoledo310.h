#ifndef __METTLER_TOLEDO_310_H__
#define __METTLER_TOLEDO_310_H__

#include "porterdriver.h"



class MettlerToledo310 : public PorterDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE void readWeight(QVariant & ret, uint &);
    void zero(uint &);


    static MettlerToledo310 * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new MettlerToledo310(drv_conf);
    }
protected:
    MettlerToledo310(const QMap<QString, QVariant>& )
    {       
    }

/*    static bool registerInFact()
    {
        factory_map().insert("MettlerToledo310", &MettlerToledo310::create);
        return true;
    }*/
private:
    //static bool registered;

    QByteArray weightRequestFrame() const;
    float parseWeightFrameAnswer(const QByteArray& ba, uint &) const;
    static BossnFactoryRegistrator<MettlerToledo310> registraror;
};

#endif
