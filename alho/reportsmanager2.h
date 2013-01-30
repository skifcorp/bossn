#ifndef REPORTSMANAGER2_H
#define REPORTSMANAGER2_H

#include "reportsmanager.h"
#include "rdb_pch.h"
#include "func.h"

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/zip.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/at_c.hpp>

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


    namespace {
        template <class E>
        struct ExprWrap
        {
        };

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

        template <class ... Args>
        void exec_args (Args ... )
        {
        }

        struct column_name_to_map2
        {
            column_name_to_map2( const QString& vn, ReportContext& rc ) : var_name(vn), rc_(rc)
            {}

            const QString& var_name;
            ReportContext& rc_;

            template <template <class, class> class Cont, class Data, template <class> class Wrapper, class Expr>
            void operator()(const Cont<Data, const Wrapper<Expr>& >& d ) const
            {
                rc_[ var_name + "_" + to_qstring(Expr::name()) ] = to_qvariant(  fusion::front(d) );
            }
        };


        template <class Seq, class WrappedExpr>
        bool call_for_each(const QString& var_name, const Seq& s, WrappedExpr we, ReportContext& rc )
        {
            fusion::for_each( fusion::zip( s, we ), column_name_to_map2{var_name, rc} );

            return true;
        }
    }

    template <class ... Conts, int ... N>
    ReportContext makeReportContext( const fusion::vector<const char(&)[N], Conts& >& ... c )
    {
        ReportContext rc;

        exec_args (
            call_for_each(
                fusion::at_c<0>(c),
                fusion::at_c<1>(c).values_,
                typename mpl::transform<
                    typename Conts::expr_list,
                    ExprWrap<mpl::_>
                >::type(), rc
            ) ...
        );


        return std::move(rc);
    }

}

#endif // REPORTSMANAGER2_H
