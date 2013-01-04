#ifndef BOSSNASYNC_H
#define BOSSNASYNC_H

#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <iostream>

#include <name_of/name_of.h>


namespace alho { namespace tools {

template <class T>
class Functor
{
private:
     T & t_;
public:
    Functor(T & t):t_(t)
    {

    }

    auto operator() () ->decltype(t_.runFunc())
    {
        return t_.runFunc();
    }

};




template <class F1, class F2>
class BossnFuture
{
private:
   F1 f1_;
   F2 f2_;
public:
    using RetType = decltype(f1_());
private:
    boost::unique_future<RetType> fut;

public:
    BossnFuture (F1  f1, F2  f2) : f1_(f1), f2_(f2)
    {

    }

    BossnFuture(BossnFuture&& other) :f1_(std::move(other.f1_)),
                                      f2_(std::move(other.f2_)),
                                      fut(std::move(other.fut))
    {

    }

    BossnFuture(const BossnFuture &) = delete;
    BossnFuture& operator=(const BossnFuture& ) = delete;

    BossnFuture & operator=( BossnFuture&& other )
    {
        f1_ = std::move(other.f1_);
        f2_ = std::move(other.f2_);
        fut = std::move(other.fut);

        return *this;
    }

    ~BossnFuture()
    {
        BOOST_ASSERT((!fut.valid()) || (fut.valid() && fut.is_ready()));
    }   

    auto runFunc() -> decltype(f1_())
    {        
        boost::this_thread::at_thread_exit(
                        boost::bind(&BossnFuture<F1, F2>::exitFunc, this));

        return f1_();
    }

    void exitFunc()
    {
        f2_();
    }

    void start()
    {
        boost::packaged_task< RetType > t( Functor<decltype(*this)>(*this) );
        fut = t.get_future();
        boost::thread th( std::move(t) );
        th.detach();
    }

    RetType get()
    {
        return  std::move(fut.get());
    }

    bool isFinished() const
    {
        return fut.is_ready();
    }

    bool hasException() const
    {
        return fut.has_exception();
    }


};

template <class F1, class F2>
BossnFuture<F1, F2> bossn_async(F1 f1, F2 f2)
{
    auto bf = BossnFuture<F1, F2>(f1, f2);
    return std::move(bf);
}


} }



#endif // BOSSNASYNC_H
