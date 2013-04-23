#ifndef MAINSEQUENCESETTINGS_H
#define MAINSEQUENCESETTINGS_H

#include <QString>

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

    void init( const QVariantMap & s )
    {
        green_light.tag_name = get_setting<QString>("green_light_tag", s);
        green_light.method_name = get_setting<QString>("green_light_method", s);
        green_light.value = get_setting<QVariant>("green_light_on_value", s);

        red_light.tag_name = get_setting<QString>("red_light_tag", s);
        red_light.method_name = get_setting<QString>("red_light_method", s);
        red_light.value = get_setting<QVariant>("red_light_on_value", s);

        tablo_tag.tag_name = get_setting<QString>("tablo_tag", s);
        tablo_tag.method_name = get_setting<QString>("tablo_method", s);

        perim_in.tag_name = get_setting<QString>("perim_in_tag", s);
        perim_in.method_name = get_setting<QString>("perim_in_method", s);
        perim_in.value = get_setting<QVariant>("perim_in_on_value", s);

        perim_out.tag_name = get_setting<QString>("perim_out_tag", s);
        perim_out.method_name = get_setting<QString>("perim_out_method", s);
        perim_out.value = get_setting<QVariant>("perim_out_on_value", s);

        weight_tag.tag_name = get_setting<QString>("weight_tag", s);
        weight_tag.method_name = get_setting<QString>("weight_method", s);

        weight_stable.tag_name = get_setting<QString>("stable_tag", s);
        weight_stable.method_name = get_setting<QString>("stable_method", s);

        logging.tag_name = get_setting<QString>("logging_tag", s);
        logging.method_name = get_setting<QString>("logging_method", s);

        reader.name                     = get_setting<QString>("reader_tag", s);
        reader.do_on.func_name          = get_setting<QString>("reader_do_on", s);
        reader.do_off.func_name         = get_setting<QString>("reader_do_off", s);
        reader.activate_idle.func_name  = get_setting<QString>("reader_activate_idle", s);
        reader.host_coded_key.func_name = get_setting<QString>("reader_host_coded_key", s);
        reader.do_auth.func_name        = get_setting<QString>("reader_do_auth", s);
        reader.write_block.func_name    = get_setting<QString>("reader_write_block", s);
        reader.read_block.func_name     = get_setting<QString>("reader_read_block", s);
        reader.do_sound.func_name       = get_setting<QString>("reader_sound", s);


    }
};

typedef BaseMainSequenceSettings<CallableTagMethod, ReaderTagMethods> MainSequenceSettings;

#endif // MAINSEQUENCESETTINGS_H
