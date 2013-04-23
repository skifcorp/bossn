#ifndef MAINSEQUENCE_H
#define MAINSEQUENCE_H

#include <vector>

using std::vector;

#include <QObject>
#include <QDebug>
#include <QTime>
#include <QApplication>
#include <QBitArray>
#include <QtConcurrentRun>
#include <QSqlDatabase>
#include <QTimer>
#include <QVector>

#include "tags.h"
#include "alhosequence.h"
#include "settingstool.h"
#include "readersettings.h"
#include "mifarereader.h"
#include "mifarecarddata.h"
#include "mainsequencesettings.h"
#include "seqdebug.h"
#include "weighter.h"
#include "cardstructs.h"
#include "mifarecard.h"
#include "weighterconf.h"
#include "photomaker.h"
#include "mainsequencebaseop.h"

class MifareCardSector;



class MainSequence : public MainSequenceBaseOp
{
    Q_OBJECT
public:
    MainSequence(Tags & t, const QVariantMap& );

    ~MainSequence() {}

    void processPerimeter();

    Q_INVOKABLE void onAppearOnWeight(const QString&, AlhoSequence*);
    Q_INVOKABLE void onDisappearOnWeight(const QString&, AlhoSequence*);

    virtual void setSettings(const QVariantMap &);

    MainSequenceSettings& alhoSettings() {return alho_settings;}
    int seqId() const {return seq_id;}
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

    QString printerName() const {return printer_name;}
    void printOnTablo(const QString& ) ;
public slots:
    void wakeUp();
private:    
    bool init;

    //Tags & tags;

    bool on_weight;

    int seq_id;

    QString printer_name;
    bool uses_photo;
    QVariantMap exit_photo;
    QVariantMap enter_photo;




    std::vector<WeighterConf> weighters_conf;

    void setLightsToRed();
    void setLightsToGreen();


    void checkForStealedCard(const ActivateCardISO14443A& );

    void initWeightersConf(const QVariantMap& s);

    WeighterConf& findWeighterConf(int) ;

    WeighterConf& readStruct(MifareCardSector&, MifareCardData& d );
    alho::common::Weighter::Pointer createWeighter (WeighterConf& wc)
    {
        return alho::common::Weighter::create( wc.weighter_name, *this, wc.database, wc.db );
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

    virtual void run();

    void makePhotoIfNeeded(long, const QString& , const WeighterConf& );
    CapturePhoto capture;
    QString current_card_tag;
    QString current_card_prop;
};


#endif // MAINSEQUENCE_H
