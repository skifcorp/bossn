#ifndef MAINSEQUENCESETTINGS_H
#define MAINSEQUENCESETTINGS_H

#include "tagmethod.h"
#include "tagfunchelper.h"
#include "alhosequence.h"
#include "tags.h"

template <bool WithCompareValue>
struct CallableTagMethod : public TagMethod<WithCompareValue>,
                           public TagFuncHelper<true>
{
    CallableTagMethod(AlhoSequence& s, Tags & t) :
        TagMethod<WithCompareValue>(),
        TagFuncHelper(TagMethod<WithCompareValue>::tag_name, TagMethod<WithCompareValue>::method_name, s, t)
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

template <template <bool WithCompare> class TagMethodType, class ReaderMethodsType>
struct BaseMainSequenceSettings
{
    TagMethodType<false> weight_tag;
    TagMethodType<false> tablo_tag;
    TagMethodType<false> weight_stable;
    TagMethodType<true> red_light;
    TagMethodType<true> green_light;
    TagMethodType<true> perim_in;
    TagMethodType<true> perim_out;
    TagMethodType<false> logging;

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
