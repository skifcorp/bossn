#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QTimer>
#include <QSharedPointer>
#include <QObject>
#include <QVariantMap>

class BaseTask : public QObject
{
public:
    typedef QSharedPointer<BaseTask> Pointer;

    BaseTask() {}

    virtual ~BaseTask(){}
    virtual void exec() = 0;
    virtual void setSettings(const QVariantMap& ) = 0;
protected:

};




#endif // TASK_H
