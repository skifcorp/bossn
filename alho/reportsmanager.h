#ifndef REPORTSDATA_H
#define REPORTSDATA_H

#include <QString>
#include <QVariant>

#include "dbstructs.h"
#include "bossnexception.h"

class ReportsManager
{
public:
    ReportsManager(const QVariantMap& s);
    ~ReportsManager(){}

    bool printReport(const qx::dao::ptr<t_ttn>&, const qx::dao::ptr<t_cars>&,  const QString&  ) const throw (MainSequenceException);
private:
    void configureReportContext(const qx::dao::ptr<t_ttn>& ttn, const qx::dao::ptr<t_cars>& car, QVariantMap& ctx) const throw (MainSequenceException);
    const QVariantMap & app_settings;
};

#endif // REPORTSDATA_H
