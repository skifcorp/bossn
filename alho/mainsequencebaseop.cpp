#include "mainsequencebaseop.h"


MainSequenceBaseOp::MainSequenceBaseOp(const QString& nm, Tags & t, const QVariantMap& s):AlhoSequence(nm),
    app_settings(s), tags(t), alho_settings(*this, tags), wake_timer(this)
{

}

MainSequenceBaseOp::~MainSequenceBaseOp()
{

}
