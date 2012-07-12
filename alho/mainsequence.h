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

#include "tags.h"
#include "alhosequence.h"
#include "settingstool.h"
#include "func.h"
#include "dbstructs.h"
#include "async_func.h"
#include "tagmethod.h"
#include "readersettings.h"
#include "conviencefuncs.h"
#include "mifarereader.h"
#include "coroutine.h"

class MifareCard;



struct MainSequenceSettings
{
    TagMethod weight_tag;
    TagMethod tablo_tag;
    TagMethod weight_stable;
    TagMethod red_light;
    TagMethod green_light;
    TagMethod perim_in;
    TagMethod perim_out;
    TagMethod logging;

    ReaderTagMethods reader;
};





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
public slots:
    void wakeUp();
private:    
    class SeqDebug : public QDebug
    {
    public:
        SeqDebug(bool use_db, MainSequence& s, int err_code) : QDebug(&buffer), use_database(use_db), seq(s), error_code(err_code) {}
        ~SeqDebug()
        {
            operator <<("platform: ").operator << (seq.seq_id).operator <<(" ");

            QString print_string = QDateTime::currentDateTimeUtc().toString("yyyy-mm-dd hh:mm:ss") + " " + QString::number(seq.seq_id).leftJustified(5) +
                    QString::number(error_code).leftJustified(5) + " " + buffer;

            if ( use_database ) {
                seq.tags[ seq.alho_settings.logging.tag_name ]->func(seq.alho_settings.logging.method_name, &seq,
                                                                            Q_ARG(const QVariant&, QVariant(seq.seq_id) ),
                                                                            Q_ARG(const QVariant&, QVariant(error_code) ),
                                                                            Q_ARG(const QVariant&, QVariant(buffer) ) );
                qWarning() <<  print_string;
            }
            else {
                qDebug() << print_string;
            }

        }
    private:
        bool use_database;
        MainSequence & seq;
        QString buffer;
        int error_code;
    };

    bool init;

    Tags & tags;    
    const QVariantMap & app_settings;


    MainSequenceSettings  alho_settings;

    bool on_weight;
    QSqlDatabase database;
    int seq_id;
    QTimer wake_timer;

    SeqDebug seqDebug(int err_code=-1)
    {
        return SeqDebug(false, *this, err_code);
    }

    SeqDebug seqWarning(int err_code=-1)
    {
        return SeqDebug(true, *this, err_code);
    }


    typedef QSharedPointer<async_func>     async_func_ptr_t;
    typedef QSharedPointer<convience_func> convience_func_ptr_t;

    async_func_ptr_t      async_func_ptr;
    convience_func_ptr_t  convience_func_ptr;

    QString detectPlatformType(const QVariantMap& ) const throw (MainSequenceException);


    void printOnTablo(const QString& ) ;
    int getWeight()  throw (MainSequenceException);


    void brutto(QVariantMap&, qx::dao::ptr<t_cars>, const MifareCard& ) throw (MainSequenceException);
    qx::dao::ptr<t_ttn> tara  (QVariantMap&, qx::dao::ptr<t_cars> )  throw (MainSequenceException) ;

    void repairBeetFieldCorrectnessIfNeeded(QVariantMap &, qx::dao::ptr<t_ttn> ) const throw();
    void processChemicalAnalysis(QVariantMap&, qx::dao::ptr<t_ttn> )  throw();
    void processFreeBum(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car ) const throw(MainSequenceException);
    void updateBruttoValues(QVariantMap&, qx::dao::ptr<t_ttn>, const MifareCard& )const throw(MainSequenceException);
    void updateTaraValues(QVariantMap&, qx::dao::ptr<t_ttn>, qx::dao::ptr<t_cars>, bool pure_weight)const throw(MainSequenceException);
    void checkLaboratory( const QVariantMap& , qx::dao::ptr<t_cars>)const throw(MainSequenceException);
    void checkKagat(const QVariantMap&) const throw(MainSequenceException);
    void checkBum(QVariantMap&) const throw(MainSequenceException);

    bool isWeightCorrect(int w) ;
    bool isPureBruttoWeight(const QVariantMap& bill) const throw (MainSequenceException);
    bool checkDeltaForReweights(int prev_weight, int cur_weight) const;

    uint countCarsFromFieldForDayExcludeCurrent(uint, uint)  throw();

    bool checkForNeedDiscreteAnalisys(long count) const throw();
    bool checkForNeedDatabaseConstAnalisys(long count, long kontrag)  throw();
    uint getAnalisysPeriodFromStorage(uint typ) const throw (MysqlException, MainSequenceException);
    QString getBumsClause(const QVariantMap & bill, qx::dao::ptr<t_cars> car) const throw(MainSequenceException);
    QString bruttoFinishMessage(const QVariantMap & bill)  const;
    QString taraFinishMessage(int)  const;

    bool isPureTaraWeight(const QVariantMap& bill)const throw (MainSequenceException);
    qx::dao::ptr<t_ttn> ttnByDriver( int  ) throw (MainSequenceException);
    void clearBumQueue(qx::dao::ptr<t_ttn> ttn) const throw (MainSequenceException);
    void processDrivingTime(qx::dao::ptr<t_ttn> , qx::dao::ptr<t_cars> )const throw (MainSequenceException);
    void repairBumCorrectnessIfNeeded( qx::dao::ptr<t_ttn> )const throw (MainSequenceException);
    bool checkBumWorks(const QDateTime& , const QDateTime&, long) const throw (MainSequenceException);
    void processTaraRupture(qx::dao::ptr<t_ttn>, qx::dao::ptr<t_cars> ) const throw (MainSequenceException);
    //qx::dao::ptr<t_const> getConst(const QString& ) const throw(MainSequenceException);
    //void configureReportContext(const qx::dao::ptr<t_ttn>&, const qx::dao::ptr<t_cars>&, QVariantMap& ) const throw (MainSequenceException);
    bool printFinishReport( const qx::dao::ptr<t_ttn>&, const qx::dao::ptr<t_cars>& ) const throw (MainSequenceException);
    bool printStartReport( const qx::dao::ptr<t_ttn>&, const qx::dao::ptr<t_cars>& ) const throw (MainSequenceException);
    void clearWorkBillData( QVariantMap& ) const;
    qx::dao::ptr<t_ttn> makeNewTask( qx::dao::ptr<t_cars>, const QVariantMap& ) const throw (MainSequenceException);

    void setLightsToRed();
    void setLightsToGreen();

    void processPerimeter()  throw (MainSequenceException);
    void checkForStealedCard(const ActivateCardISO14443A&, const ActivateCardISO14443A& ) const throw (MainSequenceException);

    template <class T>
    void setMemberValue(const QString& mn, const T& v, QVariantMap& map) const
    {
        auto iter = map.find(mn);
        if (iter == map.end()) {
            qFatal(  qPrintable("setMemberValue: cant find: " + mn) );
        }

        *iter = QVariant::fromValue<T>(v);
    }

    void setMemberValue(const QString& mn, int bit_num, bool val, QVariantMap& map) const
    {
        auto iter = map.find(mn);
        if (iter == map.end()) {
            qFatal(  qPrintable("setMemberValue: cant find: " + mn) );
        }

        QBitArray arr(bit_num + 1);
        arr.setBit(bit_num, val);

        QBitArray cur_arr = iter->toBitArray();

        *iter =  cur_arr | arr; //QVariant::fromValue<T>(v);
    }


    template <class T>
    T memberValue(const QString& mn, const QVariantMap& map) const
    {
        auto iter = map.find(mn);
        if (iter == map.end()) {
            qFatal(  qPrintable("memberValue: cant find: " + mn) );
        }
        return iter->value<T>();
    }

    template <class T>
    bool checkMember(const QString& mn, const QVariantMap& map, const T& def_val) const
    {  
        return memberValue<T>(mn, map) != def_val;
    }

    //template <>
    bool checkMember(const QString& mn, const QVariantMap& map, float def_val) const
    {
        return !qFuzzyCompare( memberValue<float>(mn, map) , def_val );
    }



    void sleepnb(int msec)
    {
       // QTime tm;
        //tm.start();
        //qApp->sendPostedEvents();
        //while (tm.elapsed() < msec) qApp->processEvents();
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
