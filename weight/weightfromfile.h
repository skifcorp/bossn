#ifndef __WEIGHT_FROM_FILE_H__
#define __WEIGHT_FROM_FILE_H__

#include "porterdriver.h"
#include <QTimer>
class WeightFromFile : public PorterDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE void readWeight(QVariant & ret, uint &);
    virtual void zero(uint &);

    static WeightFromFile * create (const QMap<QString, QVariant>& drv_conf)
    {
        return new WeightFromFile(drv_conf);
    }
protected:
    WeightFromFile(const QMap<QString, QVariant>& )
    {
    }
    /*static bool registerInFact()
    {
        factory_map().insert("WeightFromFile", &WeightFromFile::create);
        return true;
    }*/

    virtual QByteArray weightRequestFrame() const;
    virtual int parseWeightFrameAnswer(const QByteArray& ba, uint &) const;

    //static bool registered;
    static BossnFactoryRegistrator<WeightFromFile> registrator;
private:
    static int calls_counter;
};

#endif
