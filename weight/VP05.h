#ifndef __QT_VP05_H__
#define __QT_VP05_H__

#include "porterdriver.h"
#include <QDebug>

class VP05 : public PorterDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE void readWeight(QVariant & ret, uint &);
    virtual void zero(uint &);

    static VP05 * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new VP05(drv_conf);
    }

protected:
    VP05(const QMap<QString, QVariant>& )//:address(0)
    {
        //bool ok = false;
        //address = static_cast<uchar> ( conf["address"].toChar(&ok) ); Q_ASSERT(ok) ;

        qDebug () << "VP05 CREATED!";
    }

/*    static bool registerInFact()
    {
        factory_map().insert("VP05", &VP05::create);
        return true;
    }*/
 private:
    //uchar address;
    //static bool registered;

    //uchar parseAddress(const QByteArray& ba) const
    //{
    //    QByteArray ret(2, 0);
    //    ret[0] = ba[9];
    //    ret[1] = ba[10];
     //   return static_cast<uchar>(ret.toUInt(nullptr, 10));
    //}

    QByteArray weightRequestFrame() const;
    int parseWeightFrameAnswer(const QByteArray& ba, uint &) const;
    static BossnFactoryRegistrator<VP05> registrator;
};

#endif
