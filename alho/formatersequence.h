#ifndef FORMATERSEQUENCE_H
#define FORMATERSEQUENCE_H

#include "tags.h"
#include "alhosequence.h"

#include <QVariant>

class FormaterSequence : public AlhoSequence
{
public:
    FormaterSequence(Tags & t, const QVariantMap& opts):AlhoSequence("FormaterSequence"), tags(t), options(opts) {}
    ~FormaterSequence() {}

    void start();
private:
    Tags & tags;
    const QVariantMap& options;
};

#endif
