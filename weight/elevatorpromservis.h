#ifndef ELEVATORPROMSERVIS_H
#define ELEVATORPROMSERVIS_H


#ifndef __QT_ELEVATORPROMSERVIS_H__
#define __QT_ELEVATORPROMSERVIS_H__

#include "porterdriver.h"
#include <QDebug>

class EPS : public PorterDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE void readWeight(QVariant & ret, uint &);
    virtual void zero(uint &);

    static EPS * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new EPS(drv_conf);
    }

protected:
    EPS(const QMap<QString, QVariant>& )//:address(0)
    {
        //bool ok = false;
        //address = static_cast<uchar> ( conf["address"].toUInt(&ok) ); Q_ASSERT(ok) ;

        qDebug () << "EPS CREATED!";
    }

/*    static bool registerInFact()
    {
        factory_map().insert("EPS", &EPS::create);
        return true;
    }*/
 private:
    //uchar address;
    //static bool registered;

   // uchar parseAddress(const QByteArray& ba) const
   //{
   //     QByteArray ret(2, 0);
   //     ret[0] = ba[12];
   //     ret[1] = ba[13];
   //     return static_cast<uchar>(ret.toUInt(nullptr, 10));
   // }

    QByteArray weightRequestFrame() const;
    int parseWeightFrameAnswer(const QByteArray& ba, uint &) const;
    static BossnFactoryRegistrator<EPS> registrator;
};

#endif


#endif // EPS_H
