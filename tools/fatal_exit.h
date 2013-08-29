#ifndef FATAL_EXIT_H
#define FATAL_EXIT_H

#include <QDebug>

template <class Stream , class Arg>
void fatal_exit_imp( Stream && stream, Arg && arg )
{
    std::forward<Stream>(stream) << std::forward<Arg>(arg);
}

template <class Stream, class Arg, class ... Args>
void fatal_exit_imp(Stream && stream, Arg && arg,  Args&& ... args )
{
    std::forward<Stream>(stream) << std::forward<Arg>(arg);

    fatal_exit_imp(std::forward<Stream>(stream), std::forward<Args>(args)...);
}


template <class ... Args>
void fatal_exit( Args&& ... args )
{
    fatal_exit_imp(qWarning(), std::forward<Args>(args) ... );

    qFatal("exit");
}


template <class ... Args>
void fatal_assert(bool acc, Args&& ... args )
{
    if (acc)
        return;

    fatal_exit(std::forward<Args>(args) ... );

    qFatal("exit");
}


#endif // FATAL_EXIT_H
