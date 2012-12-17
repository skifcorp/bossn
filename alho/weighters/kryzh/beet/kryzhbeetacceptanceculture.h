#ifndef __BEETWEIGHTERS_H_
#define __BEETWEIGHTERS_H_

#include "acceptanceculture.h"
#include "kryzhbeetdbstructs.h"
#include "reportsmanager.h"

namespace alho  { namespace kryzh {

using alho::common::AcceptanceCulture;

class BeetAcceptanceCulture : public AcceptanceCulture
{
public:
    BeetAcceptanceCulture(MainSequence& as, QSqlDatabase& db) : AcceptanceCulture(as, db)
    {
    }

    ~BeetAcceptanceCulture() {}
    virtual void fetchCar(const MifareCardData& ) throw (MainSequenceException);
    //virtual void checkPerimetr() throw (MainSequenceException);
    virtual void brutto(int, MifareCardData& ) throw (MainSequenceException);
    virtual void tara(int, MifareCardData&) throw (MainSequenceException);
    virtual void reBrutto(int, MifareCardData&) throw (MainSequenceException);
    virtual void reTara(int, MifareCardData&) throw (MainSequenceException);

    virtual QString bruttoFinishMessage(const MifareCardData& )const;
    virtual QString taraFinishMessage(const MifareCardData& )const;

    virtual bool makeNewTask(MifareCardData& ) throw (MainSequenceException);

    virtual ReportContext finishReport( ) throw(MainSequenceException);
    virtual ReportContext startReport( ) throw(MainSequenceException);

    virtual QString detectPlatformType(const MifareCardData& ) const throw (MainSequenceException);

    virtual bool isPureBruttoWeight(const MifareCardData& ) const throw (MainSequenceException) ;
    virtual bool isPureTaraWeight(const MifareCardData& ) const throw (MainSequenceException) ;


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

};


} }

#endif
