#include "reportsmanager.h"
#include "reports.h"
#include "async_func.h"
#include "warnmessages.h"
#include "codeshacks.h"
#include "settingstool.h"
#include "conviencefuncs.h"

#include <QMessageBox>

#include "qxorm_pch.h"

#if 0
using qx::QxClassX;
using qx::IxClass;
using qx::IxDataMemberX;
using qx::IxDataMember;
#endif


#include <QTextCodec>

ReportsManager::ReportsManager()
{
}

bool ReportsManager::printReport(const ReportContext& ctx, const QString & file_name) const throw ()
{
    Reports r(file_name);
    r.setPrinterName(printer_name);
    return r.print(ctx);
}


#include <QMessageBox>

ReportContext ReportsManager::makeReportContext( QList<ReportsManager::var_instance> & vars) throw ()
{
    ReportContext ctx;
#if 0
    for( auto iter = vars.begin(); iter != vars.end(); ++iter ) {        
        IxClass * c = QxClassX::getClass( iter->typ_name );

        IxDataMemberX * m = c->getDataMemberX();
        for ( long i = 0; i < m->count(); ++i  ) {
            IxDataMember * dm = m->get(i);
                ctx[iter->var_name + "_" + dm->getName()] = dm->toVariant(  iter->var  );

        }
    }
#endif
    return ctx;
}
