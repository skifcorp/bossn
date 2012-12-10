#ifndef CULTURE_H
#define CULTURE_H



#include <QApplication>

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QSqlDatabase>

#include "QxOrm.h"

#include "bossnexception.h"
#include "mifarecarddata.h"
//#include "dbstructs.h"

//#include "reportsmanager.h"
#include "async_func.h"
#include "mifarecard.h"
//#include "mainsequence.h"


class MainSequence;
class WeighterConf;

namespace alho { namespace common {

class Culture
{
public:
    Culture(MainSequence & as, QSqlDatabase& db);
    Culture(const Culture & ) = delete;       

    virtual ~Culture() {}	
		
    void checkBruttoDeltaForReweights(int prev_weight, int cur_weight ) const throw(MainSequenceException);
    void checkTaraDeltaForReweights(int prev_weight, int cur_weight ) const throw(MainSequenceException);

    QString tr(const char * txt) const   {return qApp->translate("MainSequence", txt);}

    async_func     & asyncFunc()     {return async_    ;}
    convience_func & convienceFunc() {return convience_;}
    QSqlDatabase   & database()      {return database_ ;}
    MainSequence   & seq()           {return seq_      ;}

    const async_func     & asyncFunc() const     {return async_    ;}
    const convience_func & convienceFunc() const {return convience_;}
    const QSqlDatabase   & database() const      {return database_ ;}
    const MainSequence   & seq() const           {return seq_      ;}


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
	
    void checkWeightCorrectess(int w) throw (MainSequenceException);
    int getWeight()  throw (MainSequenceException);
 
  	

private:
    MainSequence& seq_;
    QSqlDatabase database_;
    async_func async_;
    convience_func convience_;
};

} }

#endif