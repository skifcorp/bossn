#ifndef GRAINWEIGHTERS_H
#define GRAINWEIGHTERS_H



#include "kryzhgraindbstructs.h"
#include "acceptanceculture.h"
#include "reportsmanager.h"


namespace alho { namespace kryzh {

using alho::common::AcceptanceCulture;

class GrainAcceptanceCulture : public AcceptanceCulture
{
public:
    GrainAcceptanceCulture(MainSequence& as, QSqlDatabase& db, boost::rdb::mysql::mysql_database& db2) : AcceptanceCulture(as, db, db2)
    {
    }

    ~GrainAcceptanceCulture() {}
    virtual void fetchCar(const MifareCardData& ) throw (MainSequenceException);
    virtual void brutto(int, MifareCardData& ) throw (MainSequenceException);
    virtual void tara(int, MifareCardData&) throw (MainSequenceException);
    virtual void reBrutto(int, MifareCardData& ) throw (MainSequenceException);
    virtual void reTara(int, MifareCardData& ) throw (MainSequenceException);

    virtual QString bruttoFinishMessage(const MifareCardData& )const;
    virtual QString taraFinishMessage(const MifareCardData& )const;

    bool makeNewTask(MifareCardData& bill) throw (MainSequenceException);

    virtual ReportContext finishReport( ) throw(MainSequenceException);
    virtual ReportContext startReport() throw(MainSequenceException);

    virtual QString detectPlatformType(const MifareCardData& ) const throw (MainSequenceException);
    virtual bool isPureBruttoWeight(const MifareCardData& ) const throw (MainSequenceException);
    virtual bool isPureTaraWeight(const MifareCardData& ) const throw (MainSequenceException);
    //void checkPerimetr(){}
private:
    void checkBum( const MifareCardData& )const throw(MainSequenceException);
    void checkLaboratory( qx::dao::ptr< t_ttn > )const throw(MainSequenceException);
    void updateBruttoValues(MifareCardData& bill, qx::dao::ptr<t_ttn> ttn) throw(MainSequenceException);
    void updateTaraValues(MifareCardData&, qx::dao::ptr<t_ttn>, qx::dao::ptr<t_cars>, bool pure_weight) throw(MainSequenceException);

    ReportContext makeReportContext(qx::dao::ptr<t_cars>, qx::dao::ptr<t_field>) ;


    qx::dao::ptr<t_ttn> current_ttn;
    qx::dao::ptr<t_cars> current_car;    



    static const QString t_ttn_name;
    static const QString t_cars_name;
    static const QString t_const_name;
    static const QString t_kontr_name;
    static const QString t_bum_name;
    static const QString t_kagat_name;
    static const QString t_field_name;

};


} }

#endif // GRAINWEIGHTERS_H
