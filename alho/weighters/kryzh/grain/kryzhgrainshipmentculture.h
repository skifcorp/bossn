#ifndef GRAINSHIPMENTWEIGHTER_H
#define GRAINSHIPMENTWEIGHTER_H

#include "weighter.h"
#include "kryzhgraindbstructs.h"
#include "shipmentculture.h"
#include "reportsmanager.h"

namespace alho { namespace kryzh {

using alho::common::ShipmentCulture;

class GrainShipmentCulture : public ShipmentCulture
{
public:
    GrainShipmentCulture(MainSequence& as, QSqlDatabase& db) : ShipmentCulture(as, db)
    {
    }

    ~GrainShipmentCulture() {}

    virtual void fetchCar(const MifareCardData& ) throw (MainSequenceException);
    virtual void brutto(int, MifareCardData& ) throw (MainSequenceException);
    virtual void tara(int, MifareCardData&) throw (MainSequenceException);
    virtual void reBrutto(int, MifareCardData& ) throw (MainSequenceException);
    virtual void reTara(int, MifareCardData& ) throw (MainSequenceException);

    virtual QString bruttoFinishMessage(const MifareCardData& )const;
    virtual QString taraFinishMessage(const MifareCardData& )const;

    virtual bool makeNewTask(MifareCardData& ) throw (MainSequenceException);

    virtual QString detectPlatformType(const MifareCardData& ) const throw (MainSequenceException);
    virtual bool isPureBruttoWeight(const MifareCardData& ) const throw (MainSequenceException);
    virtual bool isPureTaraWeight(const MifareCardData& ) const throw (MainSequenceException);

    virtual ReportContext finishReport( ) throw(MainSequenceException);
    //virtual ReportContext fakeReport( ) throw(MainSequenceException);
    virtual ReportContext startReport( ) throw(MainSequenceException);

private:
    void updateBruttoValues(MifareCardData& bill, qx::dao::ptr<t_ttno> ttn) throw(MainSequenceException);
    void updateTaraValues(MifareCardData&, qx::dao::ptr<t_ttno>) throw(MainSequenceException);

    qx::dao::ptr<t_prikaz>  fetchPrikaz(int) throw (MainSequenceException);
    void checkPrikazClosed(qx::dao::ptr<t_prikaz>) throw (MainSequenceException);
    qx::dao::ptr<t_ttno> ttnWithMaxNetto() throw (MainSequenceException);
    int getMaxNetto(qx::dao::ptr<t_prikaz> ) throw(MainSequenceException);
    void checkPrikazLimit(qx::dao::ptr<t_prikaz> pr) throw (MainSequenceException);
    void rollbackPrikaz( qx::dao::ptr<t_prikaz>, qx::dao::ptr<t_ttno> );

    void updatePrikaz( qx::dao::ptr<t_prikaz> ) throw (MainSequenceException);
    qx::dao::ptr<t_prikaz> makePrikaz(int) throw (MainSequenceException);

//    void makeNewTask( ) throw (MainSequenceException);
    void makeFakeTtn(int, const MifareCardData&, qx::dao::ptr<t_prikaz>) throw (MainSequenceException);

    int getVirtualNetto( qx::dao::ptr<t_prikaz> ) throw (MainSequenceException);
    void checkVirtualNetto ( qx::dao::ptr<t_prikaz> ) throw (MainSequenceException);

    ReportContext makeReportContext(qx::dao::ptr<t_ttno>) ;


    qx::dao::ptr<t_ttno> current_ttn;
    qx::dao::ptr<t_cars> current_car;

    qx::dao::ptr<t_prikaz> current_prikaz;


    static const QString t_ttn_name;
    static const QString t_cars_name;
    static const QString t_const_name;
    static const QString t_kontr_name;
    static const QString t_prikaz_name;

};

} }

#endif // GRAINSHIPMENTWEIGHTER_H
