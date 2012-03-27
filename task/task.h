#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QTimer>


#include "QSharedPointer"

class Task
{
public:
    typedef QSharedPointer<Task> Pointer;

    Task() {}
    virtual ~Task(){}
    virtual void exec() = 0;

};




#endif // TASK_H
