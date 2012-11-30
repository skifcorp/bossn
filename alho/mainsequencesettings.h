#ifndef MAINSEQUENCESETTINGS_H
#define MAINSEQUENCESETTINGS_H

#include "tagmethod.h"
#include "tagfunchelper.h"
#include "alhosequence.h"
#include "tags.h"


struct CallableTagMethod : public TagMethod,
                           public TagFuncHelper<true>
{
    CallableTagMethod(AlhoSequence& s, Tags & t) :
        TagMethod(), TagFuncHelper(TagMethod::tag_name, TagMethod::method_name, s, t)
    {}

    CallableTagMethod(const CallableTagMethod&) = delete;
};



struct ReaderTagMethods
{
    QString name;
    TagFuncHelper<false> do_on;
    TagFuncHelper<false> do_off;
    TagFuncHelper<false> activate_idle;
    TagFuncHelper<false> host_coded_key;
    TagFuncHelper<false> do_auth;
    TagFuncHelper<false> write_block;
    TagFuncHelper<false> read_block;
    TagFuncHelper<false> do_sound;
    ReaderTagMethods(AlhoSequence& s, Tags & t) : do_on(name, s, t),
        do_off(name,s,t), activate_idle(name,s, t), host_coded_key(name,s, t),
        do_auth(name,s, t), write_block(name,s, t), read_block(name,s, t), do_sound(name,s,t)
    {

    }
};

template <class TagMethodType, class ReaderMethodsType>
struct BaseMainSequenceSettings
{
    TagMethodType weight_tag;
    TagMethodType tablo_tag;
    TagMethodType weight_stable;
    TagMethodType red_light;
    TagMethodType green_light;
    TagMethodType perim_in;
    TagMethodType perim_out;
    TagMethodType logging;

    ReaderMethodsType reader;

    BaseMainSequenceSettings(AlhoSequence& s, Tags & t):weight_tag(s, t),
        tablo_tag(s, t), weight_stable(s, t), red_light(s, t),
        green_light(s, t), perim_in(s, t), perim_out(s, t), logging(s, t),
        reader(s, t)
    {

    }
};

typedef BaseMainSequenceSettings<CallableTagMethod, ReaderTagMethods> MainSequenceSettings;

#endif // MAINSEQUENCESETTINGS_H
