#ifndef __MRIA_BEET_ACCEPTANCE_CULTURE_H_
#define __MRIA_BEET_ACCEPTANCE_CULTURE_H_

#include "acceptanceculture.h"
#include "mriabeetdbstructs.h"
#include "reportsmanager.h"

namespace alho  { namespace mria {

using alho::common::AcceptanceCulture;

class BeetAcceptanceCulture : public AcceptanceCulture
{
public:
    BeetAcceptanceCulture(MainSequence& as, QSqlDatabase& db, boost::rdb::mysql::mysql_database& db2) : AcceptanceCulture(as, db, db2)
    {
    }

    ~BeetAcceptanceCulture() {}
    virtual void fetchCar(const MifareCardData& ) throw (MainSequenceException);
    virtual void checkPerimetr() throw (MainSequenceException);
    virtual void brutto(int, MifareCardData& ) throw (MainSequenceException);
    virtual void tara(int, MifareCardData&) throw (MainSequenceException);
    virtual void reBrutto(int, MifareCardData&) throw (MainSequenceException);
    virtual void reTara(int, MifareCardData&) throw (MainSequenceException);

    virtual QString bruttoFinishMessage(const MifareCardData& )const;
    virtual QString taraFinishMessage(const MifareCardData& )const;

    virtual bool makeNewTask(MifareCardData& ) throw (MainSequenceException);

    //virtual ReportContext finishReport( ) throw(MainSequenceException);
    //virtual ReportContext startReport( ) throw(MainSequenceException);

    virtual QString detectPlatformType(const MifareCardData& ) const throw (MainSequenceException);

    virtual bool isPureBruttoWeight(const MifareCardData& ) const throw (MainSequenceException) ;
    virtual bool isPureTaraWeight(const MifareCardData& ) const throw (MainSequenceException) ;


    //QString detectPlatformType(const MifareCardData& bill) const throw (MainSequenceException);


private:







    //void repairBumCorrectnessIfNeeded( qx::dao::ptr<t_ttn_beet> ) throw (MainSequenceException);
    //bool checkBumWorks(const QDateTime& , const QDateTime&, long) throw (MainSequenceException);
    //void checkLaboratory( const MifareCardData& , qx::dao::ptr<t_cars_beet>)throw(MainSequenceException);
    //void checkKagat(const MifareCardData&) throw(MainSequenceException);
    void checkBum(MifareCardData&) const throw(MainSequenceException);
    void clearBumQueue(qx::dao::ptr<t_ttn_beet_mria> ttn) throw (MainSequenceException);
    void updateBruttoValues(MifareCardData& bill, qx::dao::ptr<t_ttn_beet_mria> ttn) throw(MainSequenceException);
    void updateTaraValues(MifareCardData&, qx::dao::ptr<t_ttn_beet_mria>, qx::dao::ptr<t_cars_beet_mria>, bool pure_weight) throw(MainSequenceException);

    //bool analysisEnabled() ;

    qx::dao::ptr<t_ttn_beet_mria> current_ttn;
    qx::dao::ptr<t_cars_beet_mria> current_car;

    ReportContext makeReportContext(qx::dao::ptr<t_cars_beet_mria>, qx::dao::ptr<t_field_beet_mria>) ;



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
