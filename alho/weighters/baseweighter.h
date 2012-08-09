#ifndef BASEWEIGHTER_H
#define BASEWEIGHTER_H

#include <QApplication>

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QSqlDatabase>

#include "qxorm_pch.h"

#include "bossnexception.h"
#include "mifarecarddata.h"
//#include "dbstructs.h"
#include "factory.h"
#include "reportsmanager.h"
#include "async_func.h"
#include "mifarecard.h"
//#include "mainsequence.h"

class MainSequence;
class WeighterConf;

class BaseWeighter : //public QObject,
        public BossnFactory<BaseWeighter, MainSequence&, QSqlDatabase& >
{
public:
    typedef QSharedPointer<BaseWeighter> Pointer;

    BaseWeighter(MainSequence & as, QSqlDatabase& db);
    BaseWeighter(const BaseWeighter & ) = delete;       

    virtual ~BaseWeighter() {}

    virtual void processWeighting(MifareCardData&, MifareCard&, const WeighterConf&  ) throw (MainSequenceException, MifareCardException) = 0 ;
    virtual QString detectPlatformType(const MifareCardData& ) const throw (MainSequenceException) = 0;

    void checkBruttoDeltaForReweights(int prev_weight, int cur_weight ) const throw(MainSequenceException);
    void checkTaraDeltaForReweights(int prev_weight, int cur_weight ) const throw(MainSequenceException);

    QString tr(const char * txt) const
    {
        return qApp->translate("MainSequence", txt);
    }

    async_func * asyncFunc() {return &async_;}                //pointers for easier refactoring
    convience_func * convienceFunc() {return &convience_;}

    template <class Tttn>
    void checkTaraByBrutto(int tara, qx::dao::ptr<Tttn> ttn) const throw (MainSequenceException)
    {
        if ( tara >= ttn->brutto )
            throw MainSequenceException( tr(brutto_smaller_than_tara_message),
                "brutto smaller than tara ttn: " + QString::number(ttn->num_nakl) +
                "brutto: " + QString::number(ttn->brutto)   +
                "tara: "   + QString::number(tara) );
    }

    template <class Tttn>
    void checkBruttoByTara(int brutto, qx::dao::ptr<Tttn> ttn) const throw (MainSequenceException)
    {
        if ( brutto <= ttn->tara )
            throw MainSequenceException( tr(brutto_smaller_than_tara_message),
                "brutto smaller than tara ttn: " + QString::number(ttn->num_nakl) +
                "brutto: " + QString::number(brutto)   +
                "tara: "   + QString::number(ttn->tara) );
    }


    QString platformType(const MifareCardData & bill, const WeighterConf& ) const throw (MainSequenceException);
    void checkWeightCorrectess(int w) throw (MainSequenceException);
    int getWeight()  throw (MainSequenceException);
    void setCardBanned(const QString& ) throw();
    void checkCardBanned(const QString& ) throw(MainSequenceException);
    bool transaction();
    bool commit();
    bool rollback();
protected:
    MainSequence& seq;
    QSqlDatabase database;
    async_func async_;
    convience_func convience_;
    //template <class Tttn, class Tcars, class Tconst>
    //void processTaraRupture(qx::dao::ptr<Tttn> ttn, qx::dao::ptr<Tcars> car )  throw (MainSequenceException);
};







#endif // BASEWEIGHTER_H
