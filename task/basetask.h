#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QTimer>
#include <QSharedPointer>
#include <QObject>
#include <QVariantMap>

#include "factory.h"
#include "tags.h"


class BaseTask : public QObject,
                 public BossnFactory<BaseTask, Tags&, const QVariantMap& >
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
