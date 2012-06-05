#ifndef TASKSETTINGS_H
#define TASKSETTINGS_H


#include "portersettings.h"
#include "taskexec.h"
#include "alhosequence.h"

class TaskSettings : public PorterSettings
{
public:
    void initTasks(TaskExec& , Tags& );
    void initAlhos(QVector<AlhoSequence::Pointer>&, Tags&, const QVariantMap& );
};

#endif // TASKSETTINGS_H
