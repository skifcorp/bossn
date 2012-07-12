#ifndef PERIMETERCONTROL_H
#define PERIMETERCONTROL_H

#include <QMap>
#include <QString>
#include <QVariant>

#include "factory.h"
#include "tags.h"
#include "perimtercontrol.h"

#if 0
class PerimeterControlByDi : public PerimeterControl
{
public:
    ~PerimeterControlByDi(){}

    virtual void setSettings( const QMap<QString, QVariant>& );
    virtual bool appeared(AlhoSequence *);
    virtual bool disappeared(AlhoSequence *);

    static PerimeterControl * create(Tags & t)
    {
        return new PerimeterControlByDi(t);
    }
protected:
    PerimeterControlByDi(Tags & t) : PerimeterControl(t), prev_appear_di(false),
                                                          prev_disappear_di(false),
                                                          appear_di_value(true),
                                                          disappear_di_value(true) { }
private:
    static BossnFactoryRegistrator<PerimeterControlByDi> registator;

    bool prev_appear_di;
    bool prev_disappear_di;

    QString appear_di_name;
    QString disappear_di_name;
    bool appear_di_value;
    bool disappear_di_value;

    QString method;
};
#endif

class PerimeterControlByWeight : public PerimeterControl
{
public:
    ~PerimeterControlByWeight(){}

    virtual void setSettings( const QMap<QString, QVariant>& );
    virtual bool appeared(AlhoSequence *);
    virtual bool disappeared(AlhoSequence *);

    static PerimeterControl * create(Tags & t)
    {
        return new PerimeterControlByWeight(t);
    }
protected:
    PerimeterControlByWeight(Tags & t) : PerimeterControl(t), min_weight(0.0f), was_appeared(false) { }
private:
    static BossnFactoryRegistrator<PerimeterControlByWeight> registator;

    float min_weight;
    bool was_appeared;
    QString weight_name;

    QString get_weight_method;
};



#endif // PERIMETERCONTROL_H
