#ifndef MAINSEQUENCE_H
#define MAINSEQUENCE_H

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
//#include "func.h"
//#include "dbstructs.h"
//#include "async_func.h"
//#include "tagmethod.h"
#include "readersettings.h"
//#include "conviencefuncs.h"
#include "mifarereader.h"
//#include "coroutine.h"
#include "mifarecarddata.h"
#include "mainsequencesettings.h"
#include "seqdebug.h"
//#include "baseweighter.h"
#include "cardstructs.h"
#include "mifarecard.h"
#include "weighterconf.h"

class MifareCard;



class MainSequence : public AlhoSequence
{
//    friend class SeqDebug;

    Q_OBJECT
public:
    MainSequence(Tags & t, const QVariantMap& );

    ~MainSequence() {}


    Q_INVOKABLE void onAppearOnWeight(const QString&, AlhoSequence*);
    Q_INVOKABLE void onDisappearOnWeight(const QString&, AlhoSequence*);

    virtual void setSettings(const QVariantMap &);

    //async_func::pointer asyncFunc() {return async_func_ptr;}
    //convience_func::pointer convienceFunc() {return convience_func_ptr;}

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

    //QString platformType() const {return platform_type;}
    QString printerName() const {return printer_name;}
    void printOnTablo(const QString& ) ;
public slots:
    void wakeUp();
private:    



    bool init;

    Tags & tags;    
    const QVariantMap & app_settings;


    MainSequenceSettings  alho_settings;

    bool on_weight;
    //QSqlDatabase database;
    int seq_id;
    //QString platform_type;
    QString printer_name;

    QTimer wake_timer;




    //async_func::pointer       async_func_ptr;
    //convience_func::pointer   convience_func_ptr;


    QVector<WeighterConf> weighters_conf;




    //void brutto(MifareCardData&,  BaseWeighter::Pointer  ) throw (MainSequenceException);
    //bool tara  (MifareCardData&,  BaseWeighter::Pointer )  throw (MainSequenceException) ;


    //bool isWeightCorrect(int w) ;

    void setLightsToRed();
    void setLightsToGreen();

    void processPerimeter()  throw (MainSequenceException);
    void checkForStealedCard(const ActivateCardISO14443A&, const ActivateCardISO14443A& ) const throw (MainSequenceException);

    void initWeightersConf(const QVariantMap& s);
    //const WeighterConf& findWeighterConf(const MifareCardData&) const throw (MainSequenceException);
    WeighterConf& findWeighterConf(int) throw (MainSequenceException);

    WeighterConf& readStruct(MifareCard&, MifareCardData& d ) throw (MifareCardException, MainSequenceException);



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
};


#endif // MAINSEQUENCE_H
