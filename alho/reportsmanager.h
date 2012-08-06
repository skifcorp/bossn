#ifndef REPORTSDATA_H
#define REPORTSDATA_H

#include <QString>
#include <QVariant>

//#include "dbstructs.h"
#include "bossnexception.h"
#include "async_func.h"
#include "conviencefuncs.h"

class ReportContext : public QVariantMap
{
public:
    ReportContext(){}
    ~ReportContext(){}
};

class ReportsManager
{
public:
    struct var_instance {
        QString var_name;
        QString typ_name;
        void * var;
    };

    ReportsManager();
    ~ReportsManager(){}

    bool printReport(const ReportContext&, const QString&  ) const throw ();
    void setPrinterName(const QString& pn) {printer_name = pn;}
    QString printerName() const {return printer_name;}

    static ReportContext makeReportContext(QList<var_instance>& inst) throw ();

private:
/*    void configureReportContext(const qx::dao::ptr<t_ttn>& ttn,
                                const qx::dao::ptr<t_cars>& car,
                                const qx::dao::ptr<t_field>& field,
                                ReportContext& ctx) const throw (MainSequenceException);
*/

    QString printer_name;
};

#endif // REPORTSDATA_H
