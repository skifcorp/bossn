#ifndef ALHOSEQUENCE_H
#define ALHOSEQUENCE_H

#include <QSharedPointer>
#include <QVariant>

#include "coroutine2.h"

class AlhoSequence : public QObject, public Coroutine2
{
public:
    typedef QSharedPointer<AlhoSequence> Pointer;

    AlhoSequence(const QString& n);
    virtual ~AlhoSequence() {}

    virtual void setSettings(const QVariantMap& ) = 0;
public slots:
/*    void onShedulerFinished()
    {
        cont();
    }*/
};

#endif // ALHOSEQUENCE_H
