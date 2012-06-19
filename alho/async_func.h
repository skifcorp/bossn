#ifndef ASYNC_FUNC_H
#define ASYNC_FUNC_H

#include <QSqlDatabase>

#include <QtConcurrentRun>

//#include <QxRegister/QxClassX.h>
//#include <QxRegister/IxClass.h>
//#include <QxDataMember/IxDataMemberX.h>
//#include <QxDataMember/IxDataMember.h>
#include <QxOrm.h>

#include "bossnexception.h"

using qx::QxClassX;
using qx::IxClass;
using qx::IxDataMemberX;
using qx::IxDataMember;


class async_func
{
public:
   QSqlDatabase &database;
   async_func ( QSqlDatabase & db ) : database(db), terminate_(false) {}

   template <class Callable, class... Args >
   typename std::result_of<Callable(Args...)>::type async_call(Callable c, Args ... args)
    {
        terminate_ = false; //but this terminate_ DONT WORKS!!!!

        auto f = QtConcurrent::run(c, args...);

        while (!f.isFinished() && !terminate_ ) {
            qApp->processEvents();
        }

        terminate_ = false;

        return f.result();
    }

    void terminate(){terminate_ = true;}

    template <class T, class ID>
    qx::dao::ptr<T> async_fetch(const ID& id, bool ex_on_no_data = true) throw (MysqlException)
    {
        qx::dao::ptr<T> p = qx::dao::ptr<T>( new T(id) );

        QSqlError err = async_call([&p, this]{return qx::dao::fetch_by_id(p, &database);});

        if (err.isValid() && err.number() == 1111 && !ex_on_no_data) {
            return  qx::dao::ptr<T>();
        }

        if  (err.isValid()) {
            throw MysqlException(err.databaseText() , err.driverText());
        }
        return p;
    }

    template <class T>
    qx::dao::ptr<T> async_fetch_by_query(const qx_query& q, bool ex_on_no_data = true) throw (MysqlException)
    {
        qx::dao::ptr<T> p = qx::dao::ptr<T>( new T() );

        QSqlError err = async_call([&p, &q, this]{return qx::dao::fetch_by_query(q, p, &database);});

        if (err.isValid() && err.number() == 1111 && !ex_on_no_data) {
            return  qx::dao::ptr<T>();
        }

        if  (err.isValid()) {
            throw MysqlException(err.databaseText() , err.driverText());

        }
        return p;
    }

    template <class T>
    void async_update(qx::dao::ptr<T> p) throw (MysqlException)
    {
        QSqlError err = async_call([&p, this]{return qx::dao::update_optimized<T>(p, &database);});

        if  (err.isValid()) {
            throw MysqlException(err.databaseText(), err.driverText());
        }
    }

    template <class T>
    void async_insert(T& t) throw (MysqlException)
    {
        QSqlError err = async_call([&t, this]{return qx::dao::insert(t, &database);});

        if  (err.isValid()) {
            throw MysqlException(err.databaseText(), err.driverText());
        }
    }

    template <class T>
    void async_count(long & cnt, const qx_query& q) throw (MysqlException)
    {
        QSqlError err = async_call( [&cnt, &q, this]{ return qx::dao::count<T>(cnt, q, &database); });

        if  (err.isValid()) {
            throw MysqlException(err.databaseText() , err.driverText());
        }
    }

    template <class T>
    qx::dao::ptr<T> async_exec_query(const QString& qs, bool ex_on_no_data = true) throw (MysqlException)
    {
        qx_query q(qs);
        qx::dao::ptr<T> t = qx::dao::ptr<T>(new T);
        //T t;
        QSqlError err = async_call( [&q, &t, this]{ return qx::dao::execute_query(q, t, &database); });

        if (err.isValid() && err.number() == 1111 && !ex_on_no_data) {
            return  qx::dao::ptr<T>();
        }

        if  (err.isValid()) {
            qDebug()<<"error is valid!";
            throw MysqlException(err.databaseText() , err.driverText());
        }

        return t;
    }


    template <class T>
    T async_call_query(const QString& qs) throw (MysqlException)
    {
        qx_query q(qs);
        QSqlError err = async_call( [&q, this]{ return qx::dao::call_query( q, &database ) ;});

        if  (err.isValid()) {
            throw MysqlException(err.databaseText() , err.driverText());
        }


        QVariant v = q.getSqlResultAt(0, 0);//q.boundValue(0);
        return v.value<T>();
    }

    inline void async_call_query(const QString& qs) throw (MysqlException)
    {
        QSqlError err = async_call( [&qs, this]{ qx_query q(qs); return qx::dao::call_query( q, &database ) ;});

        if  (err.isValid()) {
            throw MysqlException(err.databaseText() , err.driverText());
        }
    }

    template <class T>
    inline QSqlError async_create_table(const QString& table_name = QString()) throw (MysqlException)
    {
        QSqlError err = async_call([&table_name, this]{ return qx::dao::create_table<T>(&database, table_name); } );
        if  (err.isValid()) {
            throw MysqlException(err.databaseText() , err.driverText());
        }
        return err;
    }

    template <class T>
    inline QSqlError async_delete_all(const QString& table_name = QString()) throw (MysqlException)
    {
        Q_UNUSED(table_name);

        QSqlError err = async_call([&table_name, this]{ return qx::dao::delete_all<T>(&database); } );
        if  (err.isValid()) {
            throw MysqlException(err.databaseText() , err.driverText());
        }
        return err;
    }


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
            //throw MainSequenceException(user_msg, admin_msg, "databaseText: " + ex.databaseText() + " driver_text: " + ex.driverText() );
        }
        return decltype(f(p...))();
    }
private:
    bool terminate_;
};

#endif // ASYNC_FUNC_H
