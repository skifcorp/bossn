#ifndef __BEETWEIGHTERS_H_
#define __BEETWEIGHTERS_H_

#include "qxorm_pch.h"

#include "acceptanceculture.h"
#include "kryzhbeetdbstructs.h"
#include "reportsmanager.h"
#include "kryzhbeettables.h"



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
    void fetchCar(const MifareCardData& ) throw (MainSequenceException);
    //virtual void checkPerimetr() throw (MainSequenceException);
    void brutto(int, MifareCardData& ) throw (MainSequenceException);
    void tara(int, MifareCardData&) throw (MainSequenceException);
    void reBrutto(int, MifareCardData&) throw (MainSequenceException);
    void reTara(int, MifareCardData&) throw (MainSequenceException);

    QString bruttoFinishMessage(const MifareCardData& )const;
    QString taraFinishMessage(const MifareCardData& )const;

    bool makeNewTask(MifareCardData& ) throw (MainSequenceException);

    ReportContext finishReport( ) throw(MainSequenceException);
    ReportContext startReport( ) throw(MainSequenceException);

    QString detectPlatformType(const MifareCardData& ) const throw (MainSequenceException);

    bool isPureBruttoWeight(const MifareCardData& ) const throw (MainSequenceException) ;
    bool isPureTaraWeight(const MifareCardData& ) const throw (MainSequenceException) ;


private:
    void processChemicalAnalysis(MifareCardData&, qx::dao::ptr<t_ttn_beet> )  throw();
    void processFreeBum(MifareCardData & bill, qx::dao::ptr<t_ttn_beet> ttn, qx::dao::ptr<t_cars_beet> car ) throw(MainSequenceException);
    uint countCarsFromFieldForDayExcludeCurrent(uint ttn_num, uint field_num)  throw();
    bool checkForNeedDatabaseConstAnalisys(long count, long kontrag)  throw ();
    uint getAnalisysPeriodFromStorage(uint typ) throw(MysqlException, MainSequenceException);
    bool checkForNeedDiscreteAnalisys(long count) const throw();
    QString getBumsClause(const MifareCardData & bill, qx::dao::ptr<t_cars_beet> car) throw(MainSequenceException);
    void repairBumCorrectnessIfNeeded( qx::dao::ptr<t_ttn_beet> ) throw (MainSequenceException);
    bool checkBumWorks(const QDateTime& , const QDateTime&, long) throw (MainSequenceException);
    void checkLaboratory( const MifareCardData& , qx::dao::ptr<t_cars_beet>)throw(MainSequenceException);
    void checkKagat(const MifareCardData&) throw(MainSequenceException);
    void checkBum(MifareCardData&) const throw(MainSequenceException);
    void clearBumQueue(qx::dao::ptr<t_ttn_beet> ttn) throw (MainSequenceException);
    void updateBruttoValues(MifareCardData& bill, qx::dao::ptr<t_ttn_beet> ttn) throw(MainSequenceException);
    void updateTaraValues(MifareCardData&, qx::dao::ptr<t_ttn_beet>, qx::dao::ptr<t_cars_beet>, bool pure_weight) throw(MainSequenceException);

    bool analysisEnabled() ;

    qx::dao::ptr<t_ttn_beet> current_ttn;
    qx::dao::ptr<t_cars_beet> current_car;



    ReportContext makeReportContext(qx::dao::ptr<t_cars_beet>, qx::dao::ptr<t_field_beet>) ;



    static const QString t_ttn_name;
    static const QString t_cars_name;
    static const QString t_const_name;
    static const QString t_kontr_name;
    static const QString t_bum_name;
    static const QString t_kagat_name;
    static const QString t_field_name;

    typename sql::table_result_set<t_cars_beet_table>::type::value_type current_car_data;
    typename sql::table_result_set<t_ttn_beet_table>::type::value_type current_ttn_data;


    t_cars_beet_table  cars_table {"t_cars"};
    t_ttn_beet_table   ttn_table{"t_ttn"};
    t_field_beet_table field_table{"t_def_field"};
};


} }



#endif
