#ifndef __MRIA_BEET_ACCEPTANCE_CULTURE_H_
#define __MRIA_BEET_ACCEPTANCE_CULTURE_H_

#include "acceptanceculture.h"
#include "mriabeetdbstructs.h"
#include "reportsmanager.h"

#include "mriabeettables.h"

//#include "rdb_pch.h"

#include <boost/mpl/bool.hpp>

#ifdef AAAA

namespace alho  { namespace mria {

using alho::common::AcceptanceCulture;

namespace sql = boost::rdb::mysql;

class BeetAcceptanceCulture : public AcceptanceCulture
{
public:
    BeetAcceptanceCulture(MainSequence& as, QSqlDatabase& db, boost::rdb::mysql::mysql_database& db2) : AcceptanceCulture(as, db, db2)
    {
    }

    ~BeetAcceptanceCulture() {}
    void fetchCar(const MifareCardData& ) ;
    void checkPerimetr() ;
    void brutto(int, MifareCardData& ) ;
    void tara(int, MifareCardData&) ;
    void reBrutto(int, MifareCardData&) ;
    void reTara(int, MifareCardData&) ;

    QString bruttoFinishMessage(const MifareCardData& )const;
    QString taraFinishMessage(const MifareCardData& )const;

    bool makeNewTask(MifareCardData& );

    //virtual ReportContext finishReport( ) throw(MainSequenceException);
    //virtual ReportContext startReport( ) throw(MainSequenceException);

    QString detectPlatformType(const MifareCardData& ) const ;

    bool isPureBruttoWeight(const MifareCardData& ) const  ;
    bool isPureTaraWeight(const MifareCardData& ) const  ;


    //QString detectPlatformType(const MifareCardData& bill) const throw (MainSequenceException);

private:
    //void repairBumCorrectnessIfNeeded( qx::dao::ptr<t_ttn_beet> ) throw (MainSequenceException);
    //bool checkBumWorks(const QDateTime& , const QDateTime&, long) throw (MainSequenceException);
    //void checkLaboratory( const MifareCardData& , qx::dao::ptr<t_cars_beet>)throw(MainSequenceException);
    //void checkKagat(const MifareCardData&) throw(MainSequenceException);
    void checkBum(MifareCardData&) const ;

    void updateBruttoValues(MifareCardData& bill);
    void updateTaraValues(MifareCardData&, boost::mpl::bool_<true> ) ;
    void updateTaraValues(MifareCardData&, boost::mpl::bool_<false> ) ;

#if 0
    void clearBumQueue(qx::dao::ptr<t_ttn_beet_mria> ttn) throw (MainSequenceException);
    void updateBruttoValues(MifareCardData& bill, qx::dao::ptr<t_ttn_beet_mria> ttn) throw(MainSequenceException);
    void updateTaraValues(MifareCardData&, qx::dao::ptr<t_ttn_beet_mria>, qx::dao::ptr<t_cars_beet_mria>, bool pure_weight) throw(MainSequenceException);

    //bool analysisEnabled() ;

    qx::dao::ptr<t_ttn_beet_mria> current_ttn;
    qx::dao::ptr<t_cars_beet_mria> current_car;

    ReportContext makeReportContext(qx::dao::ptr<t_cars_beet_mria>, qx::dao::ptr<t_field_beet_mria>) ;
#endif

    typename sql::table_result_set<t_cars_beet_table>::type::value_type current_car;
    typename sql::table_result_set<t_ttn_beet_table>::type::value_type current_ttn;

    t_cars_beet_table  cars_table {"t_cars"};
    t_ttn_beet_table   ttn_table{"t_ttn"};
    t_field_beet_table field_table{"t_field"};
    t_kagat_beet_table kagat_table{"t_kagat"};
    t_bum_beet_table   bum_table{"t_bum"};
    t_kontr_beet_table kontr_table{"t_kontr"};
    t_bum_state_log_beet_table t_bum_state_log_table{"t_bum_state_log"};
};


} }

#endif

#endif
