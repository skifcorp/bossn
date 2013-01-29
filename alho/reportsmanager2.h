#ifndef REPORTSMANAGER2_H
#define REPORTSMANAGER2_H

#include "reportsmanager.h"
#include "rdb_pch.h"
#include "func.h"

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/zip.hpp>
#include <boost/fusion/include/front.hpp>

#include <boost/mpl/transform.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/date_time/posix_time/ptime.hpp>

#include <string>

//#include <boost/fusion/include/for_each.hpp>


#include <name_of/name_of.h>

/*
class ReportsManager2
{
public:
    struct var_instance {
        QString var_name;
        QString typ_name;
        void * var;
    };

    ReportsManager2();
    ~ReportsManager2(){}

    bool printReport(const ReportContext&, const QString&  ) const ;
    void setPrinterName(const QString& pn) {printer_name = pn;}
    QString printerName() const {return printer_name;}

    static ReportContext makeReportContext(QList<var_instance>& inst);

private:

    QString printer_name; */

namespace reports {
    namespace sql    = ::boost::rdb::mysql;
    namespace fusion = ::boost::fusion;
    namespace mpl    = ::boost::mpl;

    template <class E>
    struct ExprWrap
    {
    };

    struct column_name_to_map
    {
        column_name_to_map( ReportContext& rc ) : rc_(rc)
        {}

        ReportContext& rc_;

        template <class Expr>
        void operator()( ExprWrap<Expr> )
        {
            std::cout << Expr::name() << endl;
        }
    };


    namespace {
        inline QString to_qstring(const std::string& s)
        {
            return QString::fromUtf8( s.c_str() );
        }

        template <class T>
        inline QVariant to_qvariant( const T& t )
        {
            return QVariant::fromValue(t);
        }

        inline QVariant to_qvariant( const std::string& t )
        {
            return QVariant::fromValue( to_qstring(t) );
        }

        inline QVariant to_qvariant( const boost::posix_time::ptime & t )
        {
            return QVariant::fromValue( ptime_to_qt(t) );
        }
    }

    struct column_name_to_map2
    {
        column_name_to_map2( ReportContext& rc ) : rc_(rc)
        {}

        ReportContext& rc_;

        template <template <class, class> class Cont, class Data, template <class> class Wrapper, class Expr>
        void operator()(const Cont<Data, const Wrapper<Expr>& >& d ) const
        {
            //std::cout << Expr::name() << " = " << fusion::front(d) << endl;
            rc_[ to_qstring(Expr::name()) ] = to_qvariant(  fusion::front(d) );

        }
    };

    template <class ... Args>
    void exec_args (Args ... a)
    {
        qDebug() << "args count: " << sizeof...(a);
    }

    template <class Seq, class WrappedExpr>
    bool call_for_each(const Seq& s, WrappedExpr we, ReportContext& rc )
    {
        fusion::for_each( fusion::zip( s, we ), column_name_to_map2(rc) );

        qDebug() << "called!!!!!!!!!";

        return true;
    }

    //template <class ... Seq, class ... ExprList, template <class, class>  class ... Cont>
    //ReportContext makeReportContext( const sql::nullable<Seq, ExprList>& n )
    //ReportContext makeReportContext( const Cont<Seq, ExprList>& ... n )
    template <class ... Conts>
    ReportContext makeReportContext( const Conts& ... c )
    {
        ReportContext rc;

        //using WrappedExpr = typename mpl::transform< ExprList, ExprWrap<mpl::_> >::type;

        //WrappedExpr we;

/*        exec_args (  call_for_each( c.values_,
                                    typename mpl::transform<typename Conts::expr_list, ExprWrap<mpl::_> >::type(), rc ) ...  );*/

                exec_args ( 1, 2, 3, 4  );

        qDebug() << rc;

        return std::move(rc);
    }

}

#endif // REPORTSMANAGER2_H
