#ifndef ASYNC_FUNC_H
#define ASYNC_FUNC_H

#include <QSqlDatabase>

#include <QtConcurrentRun>
#include <QFutureWatcher>

#include "qxorm_pch.h"

#include "bossnexception.h"
#include "coroutine.h"
#include <QSharedPointer>

using qx::QxClassX;
using qx::IxClass;
using qx::IxDataMemberX;
using qx::IxDataMember;

#include "warnmessages.h"

class async_func_base : public QObject
{
    Q_OBJECT
public:
    async_func_base( Coroutine & c ) : coro(c), very_busy_(false), show_debug_info_(false) {}


    template <class T, class Callable>
    struct Ret;

    template <class Callable>
    struct Ret<int, Callable>
    {
        static int value() {return 0;}
    };

    template <class Callable>
    struct Ret<bool, Callable>
    {
        static int value() {return false;}
    };

    template<class Callable>
    struct Ret<QSqlError, Callable>
    {
        static QSqlError value()
        {
            return QSqlError( "Very busy error!",
                               QString("Very busy error ") + typeid(Callable).name(),
                              QSqlError::UnknownError, 9000);
        }
    };

    template <class Callable, class... Args >
    typename std::result_of<Callable(Args...)>::type async_call(Callable c, Args ... args)
    {
        if (very_busy_) {
            qWarning( ) << "\n\nTHIS IS VERY VERY BAD CALL!!!! from: " <<
                           typeid(c).name()<<"\n\n";
            return  Ret<typename std::result_of<Callable(Args...)>::type, Callable>::value();
        }

        very_busy_ = true;

        QFutureWatcher<decltype(QtConcurrent::run(c, args...).result())> w;
        connect(&w, SIGNAL(finished()), this, SLOT(onFutureFinished()), Qt::QueuedConnection);

        auto f = QtConcurrent::run(c, args...);
        w.setFuture(f);

        while (!f.isFinished()) {
            if (show_debug_info_)
                qDebug() << "need sleep!";

            coro.yield();

            if (show_debug_info_)
                qDebug()<<"wow!! I with you!";
        }

        very_busy_ = false;

        return f.result();
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

class async_func : public async_func_base

{
    Q_OBJECT
public:
    typedef QSharedPointer<async_func> pointer;

    async_func(const async_func& ) = delete;
    async_func(QSqlDatabase& db, Coroutine & c ):
            async_func_base(c), database(db)
         {}


    template <class T, class ID>
    qx::dao::ptr<T> async_fetch(const ID& id, const QString& table_name,  bool ex_on_no_data = true) throw (MysqlException)
    {
        qx::dao::ptr<T> p = qx::dao::ptr<T>( new T(id) );

        QSqlError err = async_call([&p, this, &table_name]{return qx::dao::fetch_by_id(p, &database, table_name);});

        if (err.isValid() && err.number() == 1111 && !ex_on_no_data) {
            return  qx::dao::ptr<T>();
        }

        if  (err.isValid()) {           
            throw MysqlException(err.databaseText(),
                                 err.driverText());
        }
        return p;
    }

    template <class T>
    void async_update(qx::dao::ptr<T> p, const QString& table_name) throw (MysqlException)
    {
        QSqlError err = async_call([&p, this, &table_name]
            {return qx::dao::update_optimized<T>(p, &database, table_name);});

        if  (err.isValid()) {
            throw MysqlException(err.databaseText(),
                                 err.driverText());
        }
    }

    template <class T>
    void async_insert(T& t, bool on_duplicate_key_update=false, const QString& table_name=QString()) throw (MysqlException)
    {
        QSqlError err = async_call([&t, this, &on_duplicate_key_update, &table_name]
        {

            return qx::dao::insert(t, &database, table_name, on_duplicate_key_update);
        });

        if  (err.isValid()) {
            throw MysqlException(err.databaseText(),
                                 err.driverText());
        }
    }

    template <class T>
    void async_count(long & cnt, const qx_query& q, const QString& table_name) throw (MysqlException)
    {
        QSqlError err = async_call( [&cnt, &q, this, &table_name]
        { return qx::dao::count<T>(cnt, table_name, q, &database); });

        if  (err.isValid()) {
            throw MysqlException(err.databaseText() ,
                                 err.driverText());
        }
    }

    template <class T>
    qx::dao::ptr<T> async_exec_query(const QString& qs, bool ex_on_no_data = true) throw (MysqlException)
    {
        qx_query q(qs);
        qx::dao::ptr<T> t = qx::dao::ptr<T>(new T);
        //T t;
        QSqlError err = async_call( [&q, &t, this]
        { return qx::dao::execute_query(q, t, &database); });

        if (err.isValid() && err.number() == 1111 && !ex_on_no_data) {
            return  qx::dao::ptr<T>();
        }

        if  (err.isValid()) {
            qDebug()<<"error is valid!";
            throw MysqlException(err.databaseText() ,
                                 err.driverText());
        }

        return t;
    }


    template <class T>
    T async_call_query(const QString& qs) throw (MysqlException)
    {
        qx_query q(qs);
        QSqlError err = async_call( [&q, this]{ return qx::dao::call_query( q, &database ) ;});

        if  (err.isValid()) {
            throw MysqlException(err.databaseText() ,
                                 err.driverText() );
        }


        QVariant v = q.getSqlResultAt(0, 0);//q.boundValue(0);
        return v.value<T>();
    }

    inline void async_call_query(const QString& qs) throw (MysqlException)
    {
        QSqlError err = async_call( [&qs, this]{ qx_query q(qs); return qx::dao::call_query( q, &database ) ;});

        if  (err.isValid()) {
            throw MysqlException(err.databaseText() ,
                                 err.driverText());
        }
    }

    template <class T>
    inline QSqlError async_create_table(const QString& table_name = QString()) throw (MysqlException)
    {
        QSqlError err = async_call([&table_name, this]{
                                   //qDebug() << "before create table: " << table_name;
                                   auto ret=  qx::dao::create_table<T>(&database, table_name);
                                   //qDebug() << "after create table: " << table_name;
                                    return ret;
        } );
        if  (err.isValid()) {
            throw MysqlException(err.databaseText() ,
                                 err.driverText());
        }
        return err;
    }

    template <class T>
    inline QSqlError async_delete_all(const QString& table_name = QString()) throw (MysqlException)
    {
        Q_UNUSED(table_name);

        QSqlError err = async_call([&table_name, this]{ return qx::dao::delete_all<T>(&database); } );
        if  (err.isValid()) {
            throw MysqlException(err.databaseText() ,
                                 err.driverText());
        }
        return err;
    }

    QString databaseName() const
    {
        return database.databaseName();
    }

private:
    QSqlDatabase database;
};

template <class Func, class ... Params>
auto wrap_async_ex (const QString& user_msg, const QString& admin_msg,
                        Func f, Params ... p) throw (MainSequenceException) -> decltype( f(p...) )
{
    try {
        return f(p...);
    }
    catch (MysqlException& ex) {
        throw MainSequenceException(user_msg, admin_msg, "databaseText: " + ex.databaseText() + " driver_text: " + ex.driverText() );
    }
}

template <class Func, class ... Params>
auto wrap_async (Func f, Params ... p) throw (MainSequenceException) -> decltype( f(p...) )
{
    try {
        return f(p...);
    }
    catch (MysqlException& ex) {

    }
    return decltype(f(p...))();
}


struct convience_func
{
    typedef QSharedPointer<convience_func> pointer;

    async_func & async_func_;
    convience_func(async_func& af) : async_func_(af) {}
    convience_func(const convience_func&) = delete;

    template <class Tconst>
    inline qx::dao::ptr<Tconst> getConst(const QString & k) throw(MainSequenceException)
    {
        auto const_ = wrap_async_ex( QObject::tr(cant_get_const_message )+ QString(": ") + k, "cant get const " + k,
                 [&k,this]{return async_func_.async_fetch<Tconst>(k, "t_const");});
        return const_;
    }

};

#endif

