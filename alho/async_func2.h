#ifndef ASYNC_FUNC2_H
#define ASYNC_FUNC2_H

#include <boost/mpl/if.hpp>

#include <boost/type_traits.hpp>

#include <string>

#include "qxorm_pch.h"
#include "bossnexception.h"
#include "coroutine2.h"
#include "warnmessages.h"

#include "asyncfuncbase.h"
#include "name_of/name_of.h"

#include <boost/rdb/mysql_database.hpp>

#include <boost/bind.hpp>

#include <QObject>
#include <QDebug>

namespace mpl = boost::mpl;

namespace result_of {

    template <class Seq>
    struct fetch : mpl::if_c <
                Seq::size::value == 1,
                typename std::remove_reference<
                    typename Seq::template type_of_c<0>::type
                >::type,
                Seq
            >
    {

    };
}

template <int Size>
struct return_fetch_result
{
    template <class Seq>
    static typename std::remove_reference<Seq>::type value( Seq && s )
    {
        return std::move( s );
    }
};

template <>
struct return_fetch_result<1>
{
    template <class Seq>
    static auto value (Seq && s) -> typename std::remove_reference<decltype ( s.template get<0>() )>::type
    {
        return std::move( s.template get<0>() );
    }
};


class async_func2 : public async_func_base2
{
private:
    boost::rdb::mysql::mysql_database & database;
public:
    async_func2(  boost::rdb::mysql::mysql_database & db, Coroutine2& c  ) : async_func_base2(c), database(db)
    {

    }

    template <class Q>
    auto exec( Q&& q, const QString& user_message ) -> decltype( database.execute(q) )
    {
        return async_exec(
            [this, q]  {
                if (database.isClosed())
                    database.open();
                return database.execute(q);
            },
            user_message,
            QString::fromStdString("query: " + as_string( std::forward<Q>(q) ) + " ")
        );

    }


    template <class Q>
    auto fetch(Q && q, const QString& user_message) ->
        typename result_of::fetch<
            typename std::remove_reference<decltype( database.execute(q).all().front() )>::type
        >::type
    {
        auto deq = async_exec(
                    [this, q]  {
                        if (database.isClosed())
                            database.open();                        
                        return database.execute(q);
                    },
                    user_message,
                    QString::fromStdString("query: " + as_string( std::forward<Q>(q) ) + " ")
                ).all();

        if ( deq.size() == 0 ) {
            throw MainSequenceException( user_message,
                QString::fromStdString("query: " + as_string( std::forward<Q>(q) ) + " is empty!!!"));
        }

        using nullable_type = typename std::remove_reference<decltype(deq.front())>::type;

        return std::move( return_fetch_result< nullable_type::size::value >::value( deq.front() ) );
        //return data;
    }
};

#endif

