#ifndef MAINSEQUENCEBASEOP_H
#define MAINSEQUENCEBASEOP_H

#include <QTimer>

#include "alhosequence.h"
#include "tags.h"
#include "mainsequencesettings.h"
#include "seqdebug.h"
#include "mifarecard.h"

class MainSequenceBaseOp : public AlhoSequence
{
    Q_OBJECT
public:
    MainSequenceBaseOp(const QString& nm, Tags & t, const QVariantMap&);

    ~MainSequenceBaseOp();

    int seqId() const {return seq_id;}
    const MainSequenceSettings& alhoSettings() const {return alho_settings;}
    MainSequenceSettings& alhoSettings() {return alho_settings;}

    SeqDebug seqDebug(int err_code=-1)
    {
        return SeqDebug(false, *this, err_code);
    }

    SeqDebug seqWarning(int err_code=-1, bool to_console=true)
    {
        return SeqDebug(true, *this, err_code, to_console);
    }

    template <class T>
    T appSetting(const QString& n) const
    {
        return get_setting<T>(n, app_settings);
    }

    template <class T>
    T appSetting(const QString& n, const T& def) const
    {
        return get_setting<T>(n, app_settings, def);
    }

    void printOnTablo(const QString & s)
    {
        //tags[alho_settings.tablo_tag.tag_name]->func(alho_settings.tablo_tag.method_name, this, Q_ARG(const QVariant&, QVariant(s)));
        alho_settings.tablo_tag.func( Q_ARG(const QVariant&, QVariant(s))  );
    }
protected:
    void setLightsToRed()
    {
        alho_settings.red_light.func(Q_ARG(QVariant,  alho_settings.red_light.value.toBool() ));
        alho_settings.green_light.func(Q_ARG(QVariant, !alho_settings.green_light.value.toBool()));
    }

    void setLightsToGreen()
    {
        alho_settings.red_light.func(Q_ARG(QVariant, !alho_settings.red_light.value.toBool()));
        alho_settings.green_light.func(Q_ARG(QVariant, alho_settings.green_light.value.toBool()));
    }
    void sleepnb(int msec)
    {
        wake_timer.setInterval(msec);
        wake_timer.start();
        yield();
    }

    void sleepnbtm()
    {
        static uint tm = get_setting<uint>("sleepnb_timeout", app_settings, 100);
        sleepnb(tm);
    }

    void sleepnbtmerr(const QString& msg1, const QString& msg2)
    {
        static uint tm = get_setting<uint>("sleepnb_on_error_timeout", app_settings, 10000);
        printOnTablo(msg1);
        sleepnb(tm);
        printOnTablo(msg2);
    }

    void checkForStealedCard(const ActivateCardISO14443A& );

protected:
    const QVariantMap & app_settings;
    Tags & tags;
    MainSequenceSettings  alho_settings;
    QTimer wake_timer;
    int seq_id = 0;
protected slots:
    virtual void wakeUp( ) = 0;
};

#endif // MAINSEQUENCEBASEOP_H
