#ifndef PERIMTERCONTROL_H
#define PERIMTERCONTROL_H

#include <QMap>
#include <QVariant>

#include "tags.h"
#include "factory.h"
#include "alhosequence.h"

class PerimeterControl : public BossnFactory<PerimeterControl, Tags&>
{
public:
    virtual ~PerimeterControl(){}
    PerimeterControl(const PerimeterControl&) = delete;

    virtual void setSettings(const QMap<QString,QVariant>& ) = 0;

    virtual bool appeared(AlhoSequence *) = 0;
    virtual bool disappeared(AlhoSequence *) = 0;

    Tags & tags() {return tags_;}
    const Tags & tags() const {return tags_;}
protected:
    Tags& tags_;
    PerimeterControl(Tags & t):tags_(t){}
};

#endif // PERIMTERCONTROL_H
