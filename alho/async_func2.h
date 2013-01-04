#ifndef ASYNC_FUNC2_H
#define ASYNC_FUNC2_H

#include <string>

#include "qxorm_pch.h"
#include "bossnexception.h"
#include "coroutine.h"
#include "warnmessages.h"
#include "bossnasync.h"
#include "name_of/name_of.h"

#include <boost/rdb/mysql_database.hpp>

#include <boost/exception/all.hpp>


class async_func_base2 : public QObject
{
    Q_OBJECT
public:
    async_func_base2( Coroutine & c ) : coro(c), very_busy_(false), show_debug_info_(false)
    {}

    ~async_func_base2() {}

    template <class Callable>
    using RetType = typename std::result_of<Callable()>::type;

    template <class Callable >
    RetType<Callable> async_exec(Callable c, const QString& user_message, const QString& admin_message )
    {
        if (very_busy_) {
            throw std::runtime_error( std::string("THIS IS VERY VERY BAD CALL!!!! for: ") + name_of<Callable>::value() + " " +
                                     user_message.toStdString() + " " + admin_message.toStdString());
        }

        very_busy_ = true;

        auto bf =  alho::tools::bossn_async(c,
            [this]{
                QObject o;
                connect(&o, SIGNAL(destroyed()), this, SLOT(onFutureFinished()),Qt::QueuedConnection);
            }) ;
        bf.start();

        while (!bf.isFinished()) {
            if (show_debug_info_)
                qDebug() << "need sleep!";

            coro.yield();

            if (show_debug_info_)
                qDebug()<<"wow!! I with you!";
        }

        very_busy_ = false;                



        try {
            return std::move( bf.get() );
        }
        catch ( std::exception& ex ) {
            throw MainSequenceException( user_message, admin_message + ex.what() );
        }
        catch ( ... ) {
            throw MainSequenceException( user_message, admin_message + "unkonwn exception!!!! in asyn_exec!" );
        }

        return decltype(bf.get())() ;
    }

    void setShowDebugInfo(bool d) {show_debug_info_ = d;}
private:
    Coroutine & coro;
    bool very_busy_;
    bool show_debug_info_;
private slots:
    void onFutureFinished()
    {
        if (show_debug_info_)
            qDebug() << "FINISHED FUTURE!";

        coro.cont();

        if (show_debug_info_)
            qDebug () << "after cont!";
    }
};


class async_func2 : public async_func_base2
{
private:
    boost::rdb::mysql::mysql_database & database;
public:
    async_func2(  boost::rdb::mysql::mysql_database & db, Coroutine& c  ) : async_func_base2(c), database(db)
    {

    }

    template <class Q>
    auto exec( Q&& q, const QString& user_message ) -> decltype( database.execute(q) )
    {
        return async_exec(
            [this, q]  {
                if (database.isClosed())
                    database.open();
                auto ret = database.execute(q);
                return ret;
            },
            user_message,
            QString::fromStdString("query: " + as_string( std::forward<Q>(q) ) + " ")
        );

    }

    template <class Q>
    auto fetch(Q && q, const QString& user_message) ->
        typename std::remove_reference<decltype( database.execute(q).all().front() )>::type
    {
        auto deq = async_exec(
                    [this, q]  {
                        if (database.isClosed())
                            database.open();
                        auto ret = database.execute(q);
                        return ret;
                    },
                    user_message,
                    QString::fromStdString("query: " + as_string( std::forward<Q>(q) ) + " ")
                ).all();

        if ( deq.size() == 0 ) {
            throw MainSequenceException( user_message,
                QString::fromStdString("query: " + as_string( std::forward<Q>(q) ) + " is empty!!!"));
        }

        return std::move(deq.front());
        //return data;
    }
};

#endif

