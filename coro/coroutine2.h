#ifndef COROUTINE2_H
#define COROUTINE2_H

#include <string>
#include <memory>

using std::string;


#include <boost/context/all.hpp>
#include <boost/coroutine/all.hpp>

class Coroutine2;

namespace {
}

class Coroutine2
{
public:
    enum Status
    {
        NotStarted,
        Running,
        Stopped,
        Terminated
    };

    Coroutine2(const string& n, bool root=false);

    virtual ~Coroutine2();

    Coroutine2(const Coroutine2& ) = delete;
    Coroutine2& operator=(const Coroutine2& ) = delete;

    bool cont();
    void restart();

    static void yield();

    Status status() const
    {
        return coro_status;
    }

    template <class F>
    static Coroutine2 * build( F , const std::string& n );

    string statusText() const
    {
        switch (coro_status) {
        case NotStarted:
            return "NotStarted";
        case Running:
            return "Running";
        case Stopped:
            return "Stopped";
        case Terminated:
            return "Terminated";
        };
        return "Unknown";
    }

    void createStack(int) {}
    std::string coroName( ) const
    {
        return coro_name;
    }
protected:
    virtual void run() {}
private:
    string currentStatusText() const
    {
        return "Current status: " + statusText();
    }

    static void yieldHelper( Status  );

    static void coroEntry(intptr_t);
    static std::unique_ptr<Coroutine2> root_coro;
    static Coroutine2 * current_coro;

    Coroutine2 * caller = nullptr;

    string coro_name;
    Status coro_status = NotStarted;

    boost::context::fcontext_t * coro_context;
    boost::context::fcontext_t ret_context;

    boost::coroutines::detail::stack_allocator alloc;

    std::size_t stack_size =
            boost::coroutines::detail::stack_allocator::default_stacksize();
    void * stack = alloc.allocate( stack_size );

    void initializeContext();
};


namespace {

template <class F>
class BuildedCoroutine2 : public Coroutine2
{
public:
    BuildedCoroutine2( F f, const std::string& n ) : Coroutine2(n),  f_(f){}
protected:
    virtual void run() override
    {
        f_();
    }
private:
    F f_;
};

}

template <class F>
inline Coroutine2 * Coroutine2::build( F f, const std::string& n )
{
    return new BuildedCoroutine2<F>(f, n);
}





#endif // COROUTINE2_H
