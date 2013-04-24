#include "mainsequencebaseop.h"


MainSequenceBaseOp::MainSequenceBaseOp(const QString& nm, Tags & t, const QVariantMap& s):AlhoSequence(nm),
    app_settings(s), tags(t), alho_settings(*this, tags), wake_timer(this)
{
    connect(&wake_timer, SIGNAL(timeout()), this, SLOT(wakeUp()));
    wake_timer.setSingleShot(true);
}

MainSequenceBaseOp::~MainSequenceBaseOp()
{

}
