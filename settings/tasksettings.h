#ifndef TASKSETTINGS_H
#define TASKSETTINGS_H


#include "portersettings.h"

#include "alhosequence.h"

class TaskSettings : public PorterSettings
{
public:
    TaskSettings(const QString& fn):PorterSettings(fn){}

    void initAlhos(QVector<AlhoSequence::Pointer>&, Tags&, const QVariantMap& );
};

#endif // TASKSETTINGS_H
