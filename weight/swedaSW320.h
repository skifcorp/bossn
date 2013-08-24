#ifndef SWEDASW320_H
#define SWEDASW320_H


#ifndef __QT_SWEDA_SW_320_H__
#define __QT_SWEDA_SW_320_H__

#include "porterdriver.h"
#include <QDebug>

class SwedaSW320 : public PorterDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE void readWeight(QVariant & ret, uint &);
    virtual void zero(uint &);

    static SwedaSW320 * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new SwedaSW320(drv_conf);
    }

protected:
    SwedaSW320(const QMap<QString, QVariant>& conf):address(0)
    {
        bool ok = false;
        address = static_cast<uchar> ( conf["address"].toUInt(&ok) ); Q_ASSERT(ok) ;

        qDebug () << "SwedaSW320 CREATED!";
    }

/*    static bool registerInFact()
    {
        factory_map().insert("SwedaSW320", &SwedaSW320::create);
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
    static BossnFactoryRegistrator<SwedaSW320> registrator;
};

#endif


#endif // SwedaSW320_H
