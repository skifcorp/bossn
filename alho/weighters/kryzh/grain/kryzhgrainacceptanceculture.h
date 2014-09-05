#ifndef GRAINWEIGHTERS_H
#define GRAINWEIGHTERS_H

#include <boost/mpl/bool.hpp>

#include "kryzhgraindbstructs.h"
#include "acceptanceculture.h"
#include "reportsmanager.h"

#include "kryzhgraintables.h"
//#include "rdb_pch.h"

#ifdef AAAA

namespace alho { namespace kryzh {

using alho::common::AcceptanceCulture;

namespace sql = boost::rdb::mysql;

class GrainAcceptanceCulture : public AcceptanceCulture
{
public:
    GrainAcceptanceCulture(MainSequence& as, QSqlDatabase& db, boost::rdb::mysql::mysql_database& db2) : AcceptanceCulture(as, db, db2)
    {
    }

    ~GrainAcceptanceCulture() {}
    void fetchCar(const MifareCardData& ) ;
    void brutto(int, MifareCardData& ) ;
    void tara(int, MifareCardData&);
    void reBrutto(int, MifareCardData& );
    void reTara(int, MifareCardData& );

    QString bruttoFinishMessage(const MifareCardData& )const;
    QString taraFinishMessage(const MifareCardData& )const;

    bool makeNewTask(MifareCardData& bill) ;

    ReportContext finishReport( );
    ReportContext startReport() ;

    QString detectPlatformType(const MifareCardData& ) const;
    bool isPureBruttoWeight(const MifareCardData& ) const ;
    bool isPureTaraWeight(const MifareCardData& ) const ;
    //void checkPerimetr(){}
private:
    void checkBum( const MifareCardData& )const ;

    void checkLaboratory(  )const ;
    void updateBruttoValues(MifareCardData& bill) ;
    void updateTaraValues(MifareCardData&,  boost::mpl::bool_<true>);
    void updateTaraValues(MifareCardData&,  boost::mpl::bool_<false>);

    ReportContext makeReportContext(int field_id) ;

    typename sql::table_result_set<t_cars_grain_table>::type::value_type current_car;
    typename sql::table_result_set<t_ttn_grain_table>::type::value_type current_ttn;

    t_cars_grain_table  cars_table {"t_cars"};
    t_ttn_grain_table   ttn_table{"t_ttn"};
    t_field_grain_table field_table{"t_field"};
    t_kagat_grain_table kagat_table{"t_kagat"};
    t_bum_grain_table   bum_table{"t_bum"};
    t_kontr_grain_table kontr_table{"t_kontr"};
    t_bum_state_log_grain_table t_bum_state_log_table{"t_bum_state_log"};

};


} }

#endif

#endif // GRAINWEIGHTERS_H
