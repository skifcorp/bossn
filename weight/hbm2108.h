#ifndef __QT_HBM_2108_H__
#define __QT_HBM_2108_H__

#include "porterdriver.h"
#include <QDebug>

class Hbm2108 : public PorterDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE void readWeight(QVariant & ret, uint &);
    virtual void zero(uint &);

    static Hbm2108 * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new Hbm2108(drv_conf);
    }

protected:
    Hbm2108(const QMap<QString, QVariant>& conf):address(0)
    {
        bool ok = false;
        address = static_cast<uchar> ( conf["address"].toUInt(&ok) ); Q_ASSERT(ok) ;

        qDebug () << "Hbm2108 CREATED!";
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
        ret[0] = ba[9];
        ret[1] = ba[10];
        return static_cast<uchar>(ret.toUInt(nullptr, 10));
    }

    QByteArray weightRequestFrame() const;
    int parseWeightFrameAnswer(const QByteArray& ba, uint &) const;
    static BossnFactoryRegistrator<Hbm2108> registrator;
};

#endif
