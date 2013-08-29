#ifndef PERIMETERCONTROL_H
#define PERIMETERCONTROL_H

#include <QMap>
#include <QString>
#include <QVariant>

#include "factory.h"
#include "tags.h"
#include "perimtercontrol.h"

#include <memory>

using std::unique_ptr;

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

class SystemTrayIconEventsReceiver;

struct SystemTrayIconEventsReceiverDeleter
{
    void operator() (SystemTrayIconEventsReceiver *);
};

class PerimeterControlManualEmulator : public PerimeterControl
{
public:
    ~PerimeterControlManualEmulator(){}

    virtual void setSettings( const QMap<QString, QVariant>& );
    virtual bool appeared(AlhoSequence *);
    virtual bool disappeared(AlhoSequence *);

    static PerimeterControl * create(Tags & t)
    {
        return new PerimeterControlManualEmulator(t);
    }
protected:
    PerimeterControlManualEmulator(Tags & t);
private:
    static BossnFactoryRegistrator<PerimeterControlManualEmulator> registator;

    unique_ptr<SystemTrayIconEventsReceiver, SystemTrayIconEventsReceiverDeleter> tray;
    bool was_on_weights;
};


#include <QObject>
#include <QTimer>

class PerimeterControlCyclicEmulator : public QObject,
                                       public PerimeterControl
{
    Q_OBJECT
public:
    ~PerimeterControlCyclicEmulator(){}

    virtual void setSettings( const QMap<QString, QVariant>& );
    virtual bool appeared(AlhoSequence *);
    virtual bool disappeared(AlhoSequence *);

    static PerimeterControl * create(Tags & t)
    {
        return new PerimeterControlCyclicEmulator(t);
    }
protected:
    PerimeterControlCyclicEmulator(Tags & t);
private slots:
    void onTimer();
    void onOnPressed();
    void onOffPressed();
private:
    static BossnFactoryRegistrator<PerimeterControlCyclicEmulator> registator;

    unique_ptr<SystemTrayIconEventsReceiver, SystemTrayIconEventsReceiverDeleter> tray;
    bool was_on_weights = false;
    bool timer_processed = false;

    QTimer timer_;
};

#endif // PERIMETERCONTROL_H
