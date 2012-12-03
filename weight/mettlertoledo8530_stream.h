#ifndef MettlerToledo8530_STREAM_H
#define MettlerToledo8530_STREAM_H

#include "porterdriver.h"

class MettlerToledo8530_stream : public PorterDriver
{
    Q_OBJECT
public:
    //~MettlerToledo8530 () {}
    Q_INVOKABLE void readWeight(QVariant & ret, uint &);
    void zero(uint &);


    static MettlerToledo8530_stream * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new MettlerToledo8530_stream(drv_conf);
    }
protected:
    MettlerToledo8530_stream(const QMap<QString, QVariant>& conf):address(0)
    {
        bool ok = false;
        address = static_cast<uchar> ( conf["address"].toUInt(&ok) ); Q_ASSERT(ok) ;
    }

    virtual QByteArray weightRequestFrame() const;
    virtual float parseWeightFrameAnswer(const QByteArray& ba, uint &) const;

    virtual QByteArray zeroRequestFrame(uchar) const;
    virtual void  parseZeroFrameAnswer(const QByteArray&, uint & ) const;

    uchar address;
    //static bool registered;
    static BossnFactoryRegistrator<MettlerToledo8530_stream> registrator;
};


#endif // MettlerToledo8530_STREAM_H
