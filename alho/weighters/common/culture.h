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
#include "async_func2.h"
#include "mifarecard.h"
//#include "mainsequence.h"


class MainSequence;
class WeighterConf;

namespace alho { namespace common {

class Culture
{
public:
    Culture(MainSequence & as, QSqlDatabase& db, boost::rdb::mysql::mysql_database& db2);
    Culture(const Culture & ) = delete;       

    virtual ~Culture() {}	
		
    void checkBruttoDeltaForReweights(int prev_weight, int cur_weight ) const throw(MainSequenceException);
    void checkTaraDeltaForReweights(int prev_weight, int cur_weight ) const throw(MainSequenceException);

    QString tr(const char * txt) const   {return qApp->translate("MainSequence", txt);}

    async_func     & asyncFunc()     {return async_    ;}
    convience_func & convienceFunc() {return convience_;}
    QSqlDatabase   & database()      {return database_ ;}
    MainSequence   & seq()           {return seq_      ;}
    async_func2    & async2()        {return async2_   ;}

    const async_func     & asyncFunc() const     {return async_    ;}
    const convience_func & convienceFunc() const {return convience_;}
    const QSqlDatabase   & database() const      {return database_ ;}
    const MainSequence   & seq() const           {return seq_      ;}
    const async_func2    & async2() const        {return async2_   ;}

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


    template <class Table, class TableData>
    void checkTaraByBrutto2(int tara, const Table& t, const TableData& ttn) const throw (MainSequenceException)
    {
        if ( tara >= ttn [t.brutto] )
            throw MainSequenceException( tr(brutto_smaller_than_tara_message),
                "brutto smaller than tara ttn: " + QString::number( ttn[t.num_nakl]) +
                "brutto: " + QString::number( ttn[t.brutto])   +
                "tara: "   + QString::number(tara) );
    }

    template <class Table, class TableData>
    void checkBruttoByTara2(int brutto, const Table& t, const TableData& ttn) const throw (MainSequenceException)
    {
        if ( brutto <= ttn[t.tara] )
            throw MainSequenceException( tr(brutto_smaller_than_tara_message),
                "brutto smaller than tara ttn: " + QString::number(ttn[t.num_nakl]) +
                "brutto: " + QString::number(brutto)   +
                "tara: "   + QString::number( ttn[t.tara] ) );
    }


    void checkWeightCorrectess(int w) throw (MainSequenceException);
    int getWeight()  throw (MainSequenceException);
 
    void fillConstants();

    template <class T>
    T constantValue( const QString& n ) const
    {
        auto iter = constants_.find( n );
        if ( iter == constants_.end() ) {
            throw MainSequenceException( tr(requested_constant_not_exists) + " " + n, "requested_constant_not_exists " + n );
        }
        QVariant v( *iter );
        return v.value<T>();
    }

    bool containsConstant( const QString& n ) const
    {
        return constants_.contains(n);
    }

    const QMap<QString, QString>& constants() const
    {
        return constants_;
    }

private:
    MainSequence& seq_;
    QSqlDatabase database_;
    async_func async_;   
    convience_func convience_;
    async_func2 async2_;

private:
    QMap<QString, QString> constants_;
};

} }

#endif
