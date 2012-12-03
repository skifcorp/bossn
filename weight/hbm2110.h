#ifndef HBM2110_H
#define HBM2110_H


#ifndef __QT_HBM_2110_H__
#define __QT_HBM_2110_H__

#include "porterdriver.h"
#include <QDebug>

class Hbm2110 : public PorterDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE void readWeight(QVariant & ret, uint &);
    virtual void zero(uint &);

    static Hbm2110 * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new Hbm2110(drv_conf);
    }

protected:
    Hbm2110(const QMap<QString, QVariant>& conf):address(0)
    {
        bool ok = false;
        address = static_cast<uchar> ( conf["address"].toUInt(&ok) ); Q_ASSERT(ok) ;

        qDebug () << "Hbm2110 CREATED!";
    }

/*    static bool registerInFact()
    {
        factory_map().insert("Hbm2110", &Hbm2110::create);
        return true;
    }*/
 private:
    uchar address;
    //static bool registered;

    uchar parseAddress(const QByteArray& ba) const
    {
        QByteArray ret(2, 0);
        ret[0] = ba[12];
        ret[1] = ba[13];
        return static_cast<uchar>(ret.toUInt(nullptr, 10));
    }

    QByteArray weightRequestFrame() const;
    int parseWeightFrameAnswer(const QByteArray& ba, uint &) const;
    static BossnFactoryRegistrator<Hbm2110> registrator;
};

#endif


#endif // HBM2110_H
