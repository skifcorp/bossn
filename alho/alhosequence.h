#ifndef ALHOSEQUENCE_H
#define ALHOSEQUENCE_H

#include <QSharedPointer>
#include <QVariant>

class AlhoSequence : public QObject
{
public:
    typedef QSharedPointer<AlhoSequence> Pointer;

    AlhoSequence();
    virtual ~AlhoSequence() {}

    virtual void setSettings(const QVariantMap& ) = 0;

};

#endif // ALHOSEQUENCE_H
