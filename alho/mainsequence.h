#ifndef MAINSEQUENCE_H
#define MAINSEQUENCE_H

#include <QObject>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>

#include "tags.h"
#include "alhosequence.h"
#include "settingstool.h"
#include "func.h"
#include "dbstructs.h"

#include <QtConcurrentRun>

class MysqlException
{
public:
    MysqlException (const QString& db_txt, const QString& drv_txt):database_text(db_txt), driver_text(drv_txt)
    {

    }
    ~MysqlException () {}

    QString databaseText() const {return database_text;}
    QString driverText() const {return driver_text;}
private:
    QString database_text;
    QString driver_text;
};



class MainSequenceException
{
public:
    MainSequenceException(const QString& user_msg, const QString& admin_msg, const QString& sys_msg = QString())
        :user_message(user_msg), admin_message(admin_msg), system_message(sys_msg){}

    ~MainSequenceException() {}

    QString userMessage() const {return user_message;}
    QString adminMessage() const {return admin_message;}
    QString systemMessage() const {return system_message;}
private:
    QString user_message;
    QString admin_message;
    QString system_message;
};


class MifareCard;

class MainSequence : public QObject,
                     public AlhoSequence
{
    Q_OBJECT
public:
    MainSequence(Tags & t, const QVariantMap& opts);

    ~MainSequence() {}



    //static QByteArray card_code;
public slots:
    void onAppearOnWeight();
    void onDisappearOnWeight();
private:
    Tags & tags;
    const QVariantMap & options;

    QString tablo_func;
    QString red_tag;
    QString green_tag;
    bool on_weight;


    QString detectPlatformType(const QVariantMap& ) const throw (MainSequenceException);


    template <class Callable, class... Args >
    typename std::result_of<Callable(Args...)>::type async_call(Callable c, Args ... args) const
    {
        auto f = QtConcurrent::run(c, args...);

        while (!f.isFinished()) {
            qApp->processEvents();
        }

        return f.result();
    }

    template <class T, class ID>
    qx::dao::ptr<T> async_fetch(const ID& id) const throw (MysqlException)
    {
        qx::dao::ptr<T> p = qx::dao::ptr<T>( new T(id) );

        QSqlError err = async_call([&p]{return qx::dao::fetch_by_id(p);});

        if  (err.isValid()) {            
            throw MysqlException(err.databaseText() , err.driverText());

        }
        return p;
    }

    template <class T>
    void async_update(qx::dao::ptr<T> p) const throw (MysqlException)
    {
        QSqlError err = async_call([&p]{return qx::dao::update_optimized<T>(p);});

        if  (err.isValid()) {
            throw MysqlException(err.databaseText(), err.driverText());
        }
    }

    template <class T>
    void async_count(long & cnt, const qx_query& q) const throw (MysqlException)
    {
        QSqlError err = async_call( [&cnt, &q]{ return qx::dao::count<T>(cnt, q); });

        if  (err.isValid()) {
            throw MysqlException(err.databaseText() , err.driverText());
        }
    }

    template <class T>
    qx::dao::ptr<T> async_exec_query(const QString& qs) const throw (MysqlException)
    {
        qx_query q(qs);
        qx::dao::ptr<T> t = qx::dao::ptr<T>(new T);
        //T t;
        QSqlError err = async_call( [&q, &t]{ return qx::dao::execute_query(q, t); });

        if  (err.isValid()) {
            throw MysqlException(err.databaseText() , err.driverText());
        }

        return t;
    }


    template <class Func, class ... Params>
    auto wrap_async_ex (const QString& user_msg, const QString& admin_msg,
                        Func f, Params ... p) const throw (MainSequenceException) -> decltype( f(p...) )
    {
        try {
            return f(p...);
        }
        catch (MysqlException& ex) {
            throw MainSequenceException(user_msg, admin_msg, "databaseText: " + ex.databaseText() + " driver_text: " + ex.driverText() );
        }
    }

    template <class Func, class ... Params>
    auto wrap_async (Func f, Params ... p) const throw (MainSequenceException) -> decltype( f(p...) )
    {
        try {
            return f(p...);
        }
        catch (MysqlException& ex) {
            //throw MainSequenceException(user_msg, admin_msg, "databaseText: " + ex.databaseText() + " driver_text: " + ex.driverText() );
        }
        return decltype(f(p...))();
    }


    void printOnTablo(const QString& );
    int getWeight() const;


    void brutto(QVariantMap&, qx::dao::ptr<t_ttn>, qx::dao::ptr<t_cars> ) const throw (MainSequenceException);
    void tara  (QVariantMap&, qx::dao::ptr<t_ttn> ) const throw (MainSequenceException) ;

    void repairBeetFieldCorrectnessIfNeeded(QVariantMap &, qx::dao::ptr<t_ttn> ) const throw();
    void processChemicalAnalysis(QVariantMap&, qx::dao::ptr<t_ttn> ) const throw();
    void processFreeBum(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car ) const throw(MainSequenceException);
    void updateBruttoValues(QVariantMap&, qx::dao::ptr<t_ttn>, const MifareCard& )const throw(MainSequenceException);
    void updateTaraValues(QVariantMap&, qx::dao::ptr<t_ttn>)const throw(MainSequenceException);


    bool isWeightCorrect(int w) const {return w >= 0;}
    bool isPureBruttoWeight(const QVariantMap& bill) const throw (MainSequenceException);
    bool checkDeltaForReweights(int prev_weight, int cur_weight) const;

    uint countCarsFromFieldForDay(uint) const throw();

    bool checkForNeedDiscreteAnalisys(long count) const throw();
    bool checkForNeedDatabaseConstAnalisys(long count) const throw();
    uint getAnalisysPeriodFromStorage(uint typ) const throw (MysqlException, MainSequenceException);
    QString getBumsClause(const QVariantMap & bill, qx::dao::ptr<t_cars> car) const throw(MainSequenceException);
    QString bruttoFinishMessage(const QVariantMap & bill)  const;

    template <class T>
    void setMemberValue(const QString& mn, const T& v, QVariantMap& map) const
    {
        auto iter = map.find(mn);
        if (iter == map.end()) {
            qFatal(  qPrintable("setMemberValue: cant find: " + mn) );
        }

        *iter = QVariant::fromValue<T>(v);
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
        static uint tm = get_setting<uint>("sleepnb_timeout", options, 100);
        sleepnb(tm);
    }

    void sleepnbtmerr(const QString& msg1, const QString& msg2)
    {
        static uint tm = get_setting<uint>("sleepnb_on_error_timeout", options, 10000);
        printOnTablo(msg1);
        sleepnb(tm);
        printOnTablo(msg2);
    }

};

#endif // MAINSEQUENCE_H
