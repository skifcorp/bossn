
#ifndef GRAINSHIPMENTWEIGHTER_H
#define GRAINSHIPMENTWEIGHTER_H

#ifdef AAAA

#include "weighter.h"
#include "kryzhgraindbstructs.h"
#include "shipmentculture.h"
#include "reportsmanager.h"

//#include "rdb_pch.h"

#include "kryzhgraintables.h"



namespace alho { namespace kryzh {

using alho::common::ShipmentCulture;

namespace sql = boost::rdb::mysql;

class GrainShipmentCulture : public ShipmentCulture
{
public:
    GrainShipmentCulture(MainSequence& as, QSqlDatabase& db, boost::rdb::mysql::mysql_database& db2) : ShipmentCulture(as, db, db2)
    {
    }

    ~GrainShipmentCulture() {}

    void fetchCar(const MifareCardData& ) ;
    void brutto(int, MifareCardData& ) ;
    void tara(int, MifareCardData&) ;
    void reBrutto(int, MifareCardData& ) ;
    void reTara(int, MifareCardData& ) ;

    QString bruttoFinishMessage(const MifareCardData& )const;
    QString taraFinishMessage(const MifareCardData& )const;

    bool makeNewTask(MifareCardData& ) ;

    QString detectPlatformType(const MifareCardData& ) const ;
    bool isPureBruttoWeight(const MifareCardData& ) const ;
    bool isPureTaraWeight(const MifareCardData& ) const ;

    ReportContext finishReport( ) ;
    //virtual ReportContext fakeReport( ) throw(MainSequenceException);
    ReportContext startReport( ) ;

private:

    void updateBruttoValues(MifareCardData& bill) ;
    void updateTaraValues(MifareCardData&);

    void  fetchPrikaz(int) ;
    void checkPrikazClosed() const;
#if 0
    qx::dao::ptr<t_ttno> ttnWithMaxNetto() throw (MainSequenceException);
    int getMaxNetto(qx::dao::ptr<t_prikaz> ) throw(MainSequenceException);
#endif

    void checkPrikazLimit();
    void rollbackPrikaz();

    void updatePrikaz( );

    void makePrikaz(int);
    void updateVirtualRest();
    void updateRest();
#if 0
    //    void makeNewTask( ) throw (MainSequenceException);
    void makeFakeTtn(int, const MifareCardData&, qx::dao::ptr<t_prikaz>) throw (MainSequenceException);
#endif
    int getVirtualNetto( );
    void checkVirtualNetto (  );

    ReportContext makeReportContext() ;

#if 0
    qx::dao::ptr<t_ttno> current_ttn;
    qx::dao::ptr<t_cars> current_car;
    qx::dao::ptr<t_prikaz> current_prikaz;
#endif

    typename sql::table_result_set<t_cars_grain_table>::type::value_type current_car;
    typename sql::table_result_set<t_ttno_grain_table>::type::value_type  current_ttn;
    typename sql::table_result_set<t_prikaz_grain_table>::type::value_type  current_prikaz;

    t_cars_grain_table   cars_table{"t_cars"};
    t_ttno_grain_table   ttn_table{"t_ttno"};
    t_prikaz_grain_table prikaz_table{"t_prikaz"};
    t_kontr_grain_table kontr_table{"t_kontr"};

};

} }

#endif

#endif // GRAINSHIPMENTWEIGHTER_H
