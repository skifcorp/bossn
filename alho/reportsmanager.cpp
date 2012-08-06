#include "reportsmanager.h"
#include "reports.h"
#include "async_func.h"
#include "warnmessages.h"
#include "codeshacks.h"
#include "settingstool.h"
#include "conviencefuncs.h"

#include <QMessageBox>

#include <QxRegister/QxClassX.h>
#include <QxRegister/IxClass.h>
#include <QxDataMember/IxDataMemberX.h>
#include <QxDataMember/IxDataMember.h>

using qx::QxClassX;
using qx::IxClass;
using qx::IxDataMemberX;
using qx::IxDataMember;



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

    for( auto iter = vars.begin(); iter != vars.end(); ++iter ) {
        IxClass * c = QxClassX::getClass( iter->typ_name );

        IxDataMemberX * m = c->getDataMemberX();
        for ( long i = 0; i < m->count(); ++i  ) {
            IxDataMember * dm = m->get(i);

/*            if ( dm->getSqlType()=="TEXT" ) {
                //ctx[iter->var_name + "_" + dm->getName()] = dm->getValue<QString>(iter->var).toLatin1();
                QTextCodec * utf = QTextCodec::codecForName("UTF-8");
                QByteArray ba = utf->fromUnicode(dm->getValue<QString>(iter->var));
                QTextCodec * cp1251 = QTextCodec::codecForName("Windows-1251");

                ctx[iter->var_name + "_" + dm->getName()] = dm->getValue<QString>(iter->var);//cp1251->toUnicode(ba);
            }
            else {*/
                ctx[iter->var_name + "_" + dm->getName()] = dm->toVariant(  iter->var  );
            //}
        }
    }

    return ctx;
}
