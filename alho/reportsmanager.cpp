#include "reportsmanager.h"
#include "reports.h"
#include "async_func.h"
#include "warnmessages.h"
#include "codeshacks.h"
#include "settingstool.h"
#include "conviencefuncs.h"


#include <QxRegister/QxClassX.h>
#include <QxRegister/IxClass.h>
#include <QxDataMember/IxDataMemberX.h>
#include <QxDataMember/IxDataMember.h>

using qx::QxClassX;
using qx::IxClass;
using qx::IxDataMemberX;
using qx::IxDataMember;



#include <QTextCodec>

ReportsManager::ReportsManager(const QVariantMap &s) : app_settings(s)
{
}

bool ReportsManager::printReport(const qx::dao::ptr<t_ttn>  & ttn, const qx::dao::ptr<t_cars> & car, const QString & file_name) const throw (MainSequenceException)
{
    QVariantMap ctx;

    configureReportContext(ttn, car, ctx);

    Reports r(file_name);
    return r.print(ctx);
}



void ReportsManager::configureReportContext(const qx::dao::ptr<t_ttn>& ttn, const qx::dao::ptr<t_cars>& car, QVariantMap& ctx) const throw (MainSequenceException)
{
    qx::dao::ptr<t_field> field = wrap_async_ex(cant_get_field_when_printing, "cant get field when printing",
                            [&ttn, this]{return async_fetch<t_field>( ttn->real_field ); });

    qx::dao::ptr<t_kontr> kontr = wrap_async_ex(cant_get_kontr_when_printing, "cant get kontr when printing",
                            [&ttn, this]{return async_fetch<t_kontr>( kontrCodeFromField( ttn->real_field  ) ); });


    qx::dao::ptr<t_const> base_firm         = getConst(get_setting<QString>("base_firm_name"   , app_settings));
    qx::dao::ptr<t_const> dont_check_time   = getConst(get_setting<QString>("dont_check_time_name", app_settings));
    qx::dao::ptr<t_const> disp_phone        = getConst(get_setting<QString>("disp_phone_name", app_settings));

  struct var_instance {
        QString var_name;
        QString typ_name;
        void * var;
    };
    QList<var_instance> vars = QList<var_instance>{
        var_instance{"t_ttn", "t_ttn", ttn.data()},
        var_instance{"t_cars", "t_cars", car.data()},
        var_instance{"t_field", "t_field", field.data()},
        var_instance{"t_kontr", "t_kontr", kontr.data()},
        var_instance{"base_firm", "t_const", base_firm.data()},
        var_instance{"dont_check_time", "t_const", dont_check_time.data()},
        var_instance{"disp_phone", "t_const", disp_phone.data()}
     };

    QTextCodec * co = QTextCodec::codecForName("Windows-1251");
    //QVariantMap ctx;
    for( auto iter = vars.begin(); iter != vars.end(); ++iter ) {
        IxClass * c = QxClassX::getClass( iter->typ_name );
        //qDebug() << "name: "<<c->getName();
        IxDataMemberX * m = c->getDataMemberX();
        for ( long i = 0; i < m->count(); ++i  ) {
            IxDataMember * dm = m->get(i);
            //qDebug () << "sql_type: "<<dm->getSqlType();
            //if ( dm->getSqlType()=="TEXT" ) {
            //    ctx[iter->var_name + "_" + dm->getName()] =  co->toUnicode( dm->getValue<QString>(iter->var).toAscii() );
            //}
            //else {
                ctx[iter->var_name + "_" + dm->getName()] = dm->toVariant(  iter->var  );
            //}
        }
    }

}
