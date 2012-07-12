#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QTimer>
#include <QSharedPointer>
#include <QObject>
#include <QVariantMap>

#include "factory.h"
#include "tags.h"

#include "alhosequence.h"


class BaseTask : public AlhoSequence,
                 public BossnFactory<BaseTask, Tags&, const QVariantMap& >
{
public:
    typedef QSharedPointer<BaseTask> Pointer;

    BaseTask() {}

    virtual ~BaseTask(){}
    void exec()
    {
        restart();
        createStack(65535 * 4 );
        cont();
    }
    virtual void setSettings(const QVariantMap& ) = 0;
    virtual bool busy() const= 0;
protected:

};




#endif // TASK_H
