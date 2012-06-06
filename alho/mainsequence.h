#ifndef MAINSEQUENCE_H
#define MAINSEQUENCE_H

#include <QObject>
#include <QDebug>
#include <QTime>
#include <QApplication>
#include <QBitArray>
#include <QtConcurrentRun>

#include "tags.h"
#include "alhosequence.h"
#include "settingstool.h"
#include "func.h"
#include "dbstructs.h"
#include "async_func.h"




class MifareCard;

class MainSequence : public AlhoSequence
{
    Q_OBJECT
public:
    MainSequence(Tags & t, const QVariantMap& );

    ~MainSequence() {}


    Q_INVOKABLE void onAppearOnWeight(const QString&);
    Q_INVOKABLE void onDisappearOnWeight(const QString&);

    virtual void setSettings(const QVariantMap &);
private:
    Tags & tags;
    const QVariantMap & app_settings;


    bool on_weight;


    QString detectPlatformType(const QVariantMap& ) const throw (MainSequenceException);


    void printOnTablo(const QString& );
    int getWeight() const;


    void brutto(QVariantMap&, qx::dao::ptr<t_cars>, const MifareCard& ) const throw (MainSequenceException);
    void tara  (QVariantMap&, qx::dao::ptr<t_cars> ) const throw (MainSequenceException) ;

    void repairBeetFieldCorrectnessIfNeeded(QVariantMap &, qx::dao::ptr<t_ttn> ) const throw();
    void processChemicalAnalysis(QVariantMap&, qx::dao::ptr<t_ttn> ) const throw();
    void processFreeBum(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car ) const throw(MainSequenceException);
    void updateBruttoValues(QVariantMap&, qx::dao::ptr<t_ttn>, const MifareCard& )const throw(MainSequenceException);
    void updateTaraValues(QVariantMap&, qx::dao::ptr<t_ttn>, qx::dao::ptr<t_cars>, bool pure_weight)const throw(MainSequenceException);
    void checkLaboratory( const QVariantMap& , qx::dao::ptr<t_cars>)const throw(MainSequenceException);
    void checkKagat(const QVariantMap&) const throw(MainSequenceException);
    void checkBum(QVariantMap&) const throw(MainSequenceException);

    bool isWeightCorrect(int w) const;
    bool isPureBruttoWeight(const QVariantMap& bill) const throw (MainSequenceException);
    bool checkDeltaForReweights(int prev_weight, int cur_weight) const;

    uint countCarsFromFieldForDayExcludeCurrent(uint, uint) const throw();

    bool checkForNeedDiscreteAnalisys(long count) const throw();
    bool checkForNeedDatabaseConstAnalisys(long count, long kontrag) const throw();
    uint getAnalisysPeriodFromStorage(uint typ) const throw (MysqlException, MainSequenceException);
    QString getBumsClause(const QVariantMap & bill, qx::dao::ptr<t_cars> car) const throw(MainSequenceException);
    QString bruttoFinishMessage(const QVariantMap & bill)  const;
    QString taraFinishMessage()  const;

    bool isPureTaraWeight(const QVariantMap& bill)const throw (MainSequenceException);
    qx::dao::ptr<t_ttn> ttnByDriver( int  )const throw (MainSequenceException);
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

    void processPerimeter() const throw (MainSequenceException);

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



    void sleepnbtm() const
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



};

#endif // MAINSEQUENCE_H
