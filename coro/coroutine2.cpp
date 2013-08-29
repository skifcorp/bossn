#include "coroutine2.h"

#include <boost/assert.hpp>
#include <string>
#include "fatal_exit.h"

Coroutine2 * Coroutine2::current_coro = nullptr;

std::unique_ptr<Coroutine2> Coroutine2::root_coro;

int coro_counter = 0;

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
    if ( can_destruct_stopped )
        fatal_assert(  coro_status != Running, currentStatusText().c_str() );
    else
        fatal_assert(  coro_status == NotStarted || coro_status == Terminated, currentStatusText().c_str() );

    alloc.deallocate(stack, stack_size);
}


void Coroutine2::coroEntry(intptr_t )
{
    fatal_assert(current_coro->coro_status == Running, current_coro->currentStatusText().c_str() );

    current_coro->run();

    yieldHelper(Terminated);
}

void Coroutine2::initializeContext()
{
    coro_context = boost::context::make_fcontext( stack, stack_size, coroEntry );
}

bool Coroutine2::cont()
{    
    fatal_assert(coro_status == NotStarted || coro_status == Stopped, currentStatusText().c_str() );

    caller = current_coro;
    current_coro = this;

    current_coro->coro_status = Running;

    boost::context::jump_fcontext(&ret_context, coro_context, 0 );

    return coro_status != Terminated;
}

void Coroutine2::restart()
{
    fatal_assert(coro_status == Terminated || coro_status == NotStarted, currentStatusText().c_str() );

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
    fatal_assert( current_coro->coro_status == Running, current_coro->currentStatusText().c_str() );

    yieldHelper( Stopped );
}


