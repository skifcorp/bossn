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
    void brutto(const QVariantMap& );
    void tara  (const QVariantMap& );

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
