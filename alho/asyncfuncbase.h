#ifndef ASYNCFUNCBASE_H
#define ASYNCFUNCBASE_H

#include <QObject>
#include <QDebug>

#include <boost/mpl/bool.hpp>
namespace mpl = boost::mpl;


#include "coroutine2.h"
#include "name_of/name_of.h"
#include "bossnasync.h"
#include "bossnexception.h"

class async_func_base2 : public QObject
{
    Q_OBJECT
public:
    async_func_base2( Coroutine2 & c ) : coro(c), very_busy_(false), show_debug_info_(false)
    {}

    ~async_func_base2() {}



    template <class Callable>
    using RetType = typename std::result_of<Callable()>::type;

    template <class Callable >
    RetType<Callable> async_exec(Callable c, const QString& user_message, const QString& admin_message )
    {
        return async_exec_imp( c, user_message, admin_message,
                              typename boost::is_same<void, RetType<Callable> >::type() );
    }

    void setShowDebugInfo(bool d) {show_debug_info_ = d;}
private:
    Coroutine2 & coro;
    bool very_busy_;

    bool show_debug_info_;

    void disconnector()
    {
        QObject o;
        connect(&o, SIGNAL(destroyed()), this, SLOT(onFutureFinished()),Qt::QueuedConnection);
    }

    template <class Callable >
    RetType<Callable> async_exec_imp(Callable c, const QString& user_message, const QString& admin_message, boost::mpl::bool_<false> )
    {
        if (very_busy_) {
            throw std::runtime_error( std::string("THIS IS VERY VERY BAD CALL!!!! for: ") + name_of<Callable>::value() + " " +
                                     user_message.toStdString() + " " + admin_message.toStdString());
        }

        very_busy_ = true;

        auto bf =  alho::tools::bossn_async(c, boost::bind(&async_func_base2::disconnector, this));
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
            throw MainSequenceException( user_message, admin_message + "unkonwn exception!!!! in async_exec!" );
        }
    }

    template <class Callable >
    void async_exec_imp(Callable c, const QString& user_message, const QString& admin_message, boost::mpl::bool_<true> )
    {
        if (very_busy_) {
            throw std::runtime_error( std::string("THIS IS VERY VERY BAD CALL!!!! for: ") + name_of<Callable>::value() + " " +
                                     user_message.toStdString() + " " + admin_message.toStdString());
        }

        very_busy_ = true;

        auto bf =  alho::tools::bossn_async(c, boost::bind(&async_func_base2::disconnector, this));
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
           bf.get();
        }
        catch ( std::exception& ex ) {
            throw MainSequenceException( user_message, admin_message + ex.what() );
        }
        catch ( ... ) {
            throw MainSequenceException( user_message, admin_message + "unkonwn exception!!!! in async_exec!" );
        }

/*        try {
            bf.get();
        }
        catch ( std::exception& ex ) {
            throw MainSequenceException( user_message, admin_message + ex.what() );
        }
        catch ( ... ) {
            throw MainSequenceException( user_message, admin_message + "unkonwn exception!!!! in async_exec!" );
        }*/
    }


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


#endif // ASYNCFUNCBASE_H
