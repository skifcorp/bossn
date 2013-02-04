#include "coroutine2.h"

#include <boost/assert.hpp>
#include <string>

Coroutine2 * Coroutine2::current_coro = nullptr;

std::unique_ptr<Coroutine2> Coroutine2::root_coro;

Coroutine2::Coroutine2(const string& n, bool root) : coro_name(n)
{
    if ( !root_coro && !root ) {
        root_coro.reset( new Coroutine2( "Root", true ) );
        current_coro = root_coro.get();
    }

    initializeContext();
}

Coroutine2::~Coroutine2()
{
    BOOST_ASSERT_MSG(  coro_status == NotStarted || coro_status == Terminated, currentStatusText().c_str() );
}


void Coroutine2::coroEntry(intptr_t )
{
    BOOST_ASSERT_MSG(current_coro->coro_status == Running, current_coro->currentStatusText().c_str() );

    current_coro->run();

    yieldHelper(Terminated);
}

void Coroutine2::initializeContext()
{
    coro_context = boost::context::make_fcontext( stack, stack_size, coroEntry );
}

bool Coroutine2::cont()
{    
    BOOST_ASSERT_MSG(coro_status == NotStarted || coro_status == Stopped, currentStatusText().c_str() );

    caller = current_coro;
    current_coro = this;

    current_coro->coro_status = Running;

    boost::context::jump_fcontext(&ret_context, coro_context, 0 );

    return coro_status != Terminated;
}

void Coroutine2::restart()
{
    BOOST_ASSERT_MSG(coro_status == Terminated || coro_status == NotStarted, currentStatusText().c_str() );

    initializeContext();

    coro_status = NotStarted;
}

void Coroutine2::yieldHelper( Status  s )
{
    current_coro->coro_status = s;

    Coroutine2 * c = current_coro;
    current_coro = c->caller;
    boost::context::jump_fcontext(c->coro_context,
        &c->ret_context, 0 );
}

void Coroutine2::yield()
{
    BOOST_ASSERT_MSG( current_coro->coro_status == Running, current_coro->currentStatusText().c_str() );

    yieldHelper( Stopped );
}


