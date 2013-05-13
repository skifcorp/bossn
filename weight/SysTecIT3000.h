#ifndef __QT_SysTecIT3000_H__
#define __QT_SysTecIT3000_H__

#include "porterdriver.h"
#include <QDebug>

class SysTecIT3000 : public PorterDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE void readWeight(QVariant & ret, uint &);
    virtual void zero(uint &);

    static SysTecIT3000 * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new SysTecIT3000(drv_conf);
    }

protected:
    SysTecIT3000(const QMap<QString, QVariant>& conf):address(0)
    {
        bool ok = false;
        address = static_cast<uchar> ( conf["address"].toUInt(&ok) ); Q_ASSERT(ok) ;

        qDebug () << "SysTecIT3000 CREATED!";
    }

/*    static bool registerInFact()
    {
        factory_map().insert("SysTecIT3000", &SysTecIT3000::create);
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
    static BossnFactoryRegistrator<SysTecIT3000> registrator;
};

#endif
