#ifndef MAINSEQUENCE_H
#define MAINSEQUENCE_H

#include <QObject>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>

#include "tags.h"
#include "alhosequence.h"

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



    //bool checkBill (const QVariantMap& ) const;
    template <class T>
    bool checkMember(const QString& mn, const QVariantMap& map, const T& def_val)
    {
        auto iter = map.find(mn);
        if (iter == map.end()) {
            qWarning() << "checkMember: cant find"<<mn;
            return false;
        }
        T ret = iter->value<T>();
        return ret != def_val;
    }
};

#endif // MAINSEQUENCE_H
