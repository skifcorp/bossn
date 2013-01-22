#ifndef __BEETWEIGHTERS_H_
#define __BEETWEIGHTERS_H_

//#include "qxorm_pch.h"

#include "rdb_pch.h"

#include "acceptanceculture.h"
#include "kryzhbeetdbstructs.h"
#include "reportsmanager.h"
#include "kryzhbeettables.h"

#include <boost/mpl/bool.hpp>


namespace alho  { namespace kryzh {

using alho::common::AcceptanceCulture;

namespace sql = boost::rdb::mysql;

class BeetAcceptanceCulture : public AcceptanceCulture
{
public:
    BeetAcceptanceCulture(MainSequence& as, QSqlDatabase& db, boost::rdb::mysql::mysql_database& db2) : AcceptanceCulture(as, db, db2)
    {        
    }

    ~BeetAcceptanceCulture() {}
    void fetchCar(const MifareCardData& );
    //virtual void checkPerimetr() throw (MainSequenceException);
    void brutto(int, MifareCardData& );
    void tara(int, MifareCardData&);
    void reBrutto(int, MifareCardData&);
    void reTara(int, MifareCardData&) ;

    QString bruttoFinishMessage(const MifareCardData& )const;
    QString taraFinishMessage(const MifareCardData& )const;

    bool makeNewTask(MifareCardData& ) ;

    ReportContext finishReport( ) ;
    ReportContext startReport( ) ;

    QString detectPlatformType(const MifareCardData& ) const ;

    bool isPureBruttoWeight(const MifareCardData& ) const  ;
    bool isPureTaraWeight(const MifareCardData& ) const  ;
private:
    void processChemicalAnalysis(MifareCardData& ) ;
    void processFreeBum(MifareCardData & bill );
    uint countCarsFromFieldForDayExcludeCurrent(int field_num);
    bool checkForNeedDatabaseConstAnalisys(long count, long kontrag)  ;
    uint getAnalisysPeriodFromStorage(uint typ);
    bool checkForNeedDiscreteAnalisys(long count) const;
    QString getBumsClause(const MifareCardData & bill);
    void repairBumCorrectnessIfNeeded(  ) ;
    bool checkBumWorks(const QDateTime& , const QDateTime&, long) ;
    void checkLaboratory( const MifareCardData& );
    void checkKagat(const MifareCardData&) ;
    void checkBum(MifareCardData&) const ;
    void clearBumQueue() ;
    void updateBruttoValues(MifareCardData& bill) ;
    void updateTaraValues(MifareCardData&, boost::mpl::bool_<true>);
    void updateTaraValues(MifareCardData&, boost::mpl::bool_<false>);

    bool analysisEnabled() ;

    //qx::dao::ptr<t_ttn_beet> current_ttn;
    //qx::dao::ptr<t_cars_beet> current_car;



    ReportContext makeReportContext(qx::dao::ptr<t_cars_beet>, qx::dao::ptr<t_field_beet>) ;



    static const QString t_ttn_name;
    static const QString t_cars_name;
    static const QString t_const_name;
    static const QString t_kontr_name;
    static const QString t_bum_name;
    static const QString t_kagat_name;
    static const QString t_field_name;

    typename sql::table_result_set<t_cars_beet_table>::type::value_type current_car;
    typename sql::table_result_set<t_ttn_beet_table>::type::value_type current_ttn;



    t_cars_beet_table  cars_table {"t_cars"};
    t_ttn_beet_table   ttn_table{"t_ttn"};
    t_field_beet_table field_table{"t_def_field"};
    t_kagat_beet_table kagat_table{"t_kagat"};
    t_bum_beet_table   bum_table{"t_bum"};
    t_kontr_beet_table kontr_table{"t_kontr"};

};


} }



#endif
