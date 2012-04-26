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


    QString detectPlatformType(const QVariantMap& ) const;

    template <class T>
    qx::dao::ptr<T> async_fetch(long id) const
    {
        qx::dao::ptr<T> p = qx::dao::ptr<T>( new T(id) );

        QSqlError err = async_call([&p]{return qx::dao::fetch_by_id(p);});

        if  (err.isValid()) {
            qWarning( ) << "failed fetch: "<<err.databaseText()<<" "<<err.driverText();
            return qx::dao::ptr<T>();
        }
        return p;
    }

    template <class T>
    bool async_update(const qx::dao::ptr<T>& p) const
    {
        QSqlError err = async_call([&p]{return qx::dao::update_optimized(p);});

        if  (err.isValid()) {
            qWarning( ) << "failed update: "<<err.databaseText()<<" "<<err.driverText();
            return false;
        }
        return true;
    }


    void printOnTablo(const QString& );

    void brutto(QVariantMap& );
    void tara  (QVariantMap& );

    void checkBeetFieldCorrectness(QVariantMap &, qx::dao::ptr<t_ttn>  );


    template <class T>
    T memberValue(const QString& mn, const QVariantMap& map) const
    {
        auto iter = map.find(mn);
        if (iter == map.end()) {
            qFatal(  qPrintable("memberValue: cant find" + mn) );
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
};

#endif // MAINSEQUENCE_H
