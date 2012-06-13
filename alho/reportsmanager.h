#ifndef REPORTSDATA_H
#define REPORTSDATA_H

#include <QString>
#include <QVariant>

#include "dbstructs.h"
#include "bossnexception.h"
#include "async_func.h"
#include "conviencefuncs.h"

class ReportsManager
{
public:
    ReportsManager(const QVariantMap& s, async_func& af, convience_func& cf);
    ~ReportsManager(){}

    bool printReport(const qx::dao::ptr<t_ttn>&, const qx::dao::ptr<t_cars>&, const qx::dao::ptr<t_field>& field, const QString&  ) const throw (MainSequenceException);
private:
    void configureReportContext(const qx::dao::ptr<t_ttn>& ttn,
                                const qx::dao::ptr<t_cars>& car,
                                const qx::dao::ptr<t_field>& field,
                                QVariantMap& ctx) const throw (MainSequenceException);
    const QVariantMap & app_settings;
    async_func     & async_func_;
    convience_func & convience_func_;
};

#endif // REPORTSDATA_H
