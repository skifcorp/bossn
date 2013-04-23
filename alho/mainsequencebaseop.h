#ifndef MAINSEQUENCEBASEOP_H
#define MAINSEQUENCEBASEOP_H

#include <QTimer>

#include "alhosequence.h"
#include "tags.h"
#include "mainsequencesettings.h"

class MainSequenceBaseOp : public AlhoSequence
{
public:
    MainSequenceBaseOp(const QString& nm, Tags & t, const QVariantMap&);

    ~MainSequenceBaseOp();
protected:
    const QVariantMap & app_settings;
    Tags tags;
    MainSequenceSettings  alho_settings;
    QTimer wake_timer;
};

#endif // MAINSEQUENCEBASEOP_H
