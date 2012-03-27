#ifndef TASKEXEC_H
#define TASKEXEC_H


#include <QObject>
#include <QTimer>
#include <QSharedPointer>
#include <QMap>
#include <QDebug>


#include "task.h"


class TaskClass : public QObject
{
    Q_OBJECT
public:
    typedef QSharedPointer<TaskClass> Pointer;

    TaskClass(int period)
    {
        timer = QSharedPointer<QTimer>(new QTimer);
        timer->setInterval(period);
        connect(timer.data(), SIGNAL(timeout()), this, SLOT(onTimer()));
        timer->start();
    }
    TaskClass(const TaskClass& ) = delete;

    ~TaskClass() {}
    void addTask(Task::Pointer t)
    {
        tasks.append(t);
    }

private slots:
    void onTimer()
    {
        qDebug () << "!!!!!!!!!!!!!!!!!!!!!!!";

        for (QList<Task::Pointer>::iterator iter = tasks.begin(); iter != tasks.end(); ++iter) {
            (*iter)->exec();
        }
    }
private:
    QSharedPointer<QTimer> timer;
    QList<Task::Pointer> tasks;
};

class TaskExec
{
public:
    TaskExec()
    {

    }
    ~TaskExec() {}
    void addTask(int period, Task::Pointer t)
    {
        auto iter = task_classes.find(period);
        if (iter == task_classes.end()) {
            TaskClass::Pointer tc(new TaskClass(period));
            tc->addTask(t);
            task_classes.insert(period, tc);
        }
        else {
            (*iter)->addTask(t);
        }
    }
private:
    QMap<int, TaskClass::Pointer> task_classes;
};

#endif // TASKEXEC_H
