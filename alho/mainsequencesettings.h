#ifndef MAINSEQUENCESETTINGS_H
#define MAINSEQUENCESETTINGS_H

#include <QString>

#include "tagmethod.h"
#include "tagfunchelper.h"
#include "tagprophelper.h"
#include "alhosequence.h"
#include "tags.h"

#include <boost/mpl/if.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/int.hpp>

#include <type_traits>

#include <name_of/name_of.h>

namespace mpl = boost::mpl;



template <TagMethodType T>
using IsTagWithFunc = typename mpl::or_<
                    typename mpl::equal_to<
                        mpl::int_<static_cast<int>(T)>,
                        mpl::int_<static_cast<int>(TagMethodType::Simple)>
                    >::type,
                    typename mpl::equal_to<
                        mpl::int_<static_cast<int>(T)>,
                        mpl::int_<static_cast<int>(TagMethodType::CompareValue)>
                    >::type
                >;



template <TagMethodType Typ>
struct CallableTagMethod : public TagMethod<Typ>,
                           public mpl::if_c<
                                        IsTagWithFunc<Typ>::value,
                                        TagFuncHelper<true>,
                                        TagPropHelper<false>
                                    >::type

{

    template <TagMethodType Typ2 = Typ>
    CallableTagMethod(AlhoSequence& s, Tags & t,  typename std::enable_if<IsTagWithFunc<Typ2>::value>::type * v = 0 ) :
        TagMethod<Typ>(),
        TagFuncHelper<true>(TagMethod<Typ>::tag_name, TagMethod<Typ>::method_name, s, t)
    {
        Q_UNUSED(v);
    }

    template <TagMethodType Typ2 = Typ>
    CallableTagMethod(Tags & t,  typename std::enable_if<!IsTagWithFunc<Typ2>::value>::type * v = 0 ) :
        TagMethod<Typ>(),
        TagPropHelper<false>(TagMethod<Typ>::tag_name, t)
    {
        Q_UNUSED(v);
    }


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

template <template <TagMethodType Typ> class TagMethodTyp, class ReaderMethodsType>
struct BaseMainSequenceSettings
{
    TagMethodTyp<TagMethodType::Simple> weight_tag;
    TagMethodTyp<TagMethodType::Simple> tablo_tag;
    TagMethodTyp<TagMethodType::Simple> weight_stable;
    TagMethodTyp<TagMethodType::CompareValue> red_light;
    TagMethodTyp<TagMethodType::CompareValue> green_light;
    TagMethodTyp<TagMethodType::CompareValue> perim_in;
    TagMethodTyp<TagMethodType::CompareValue> perim_out;
    TagMethodTyp<TagMethodType::Simple> logging;
    TagMethodTyp<TagMethodType::Property> current_card;

    ReaderMethodsType reader;

    BaseMainSequenceSettings(AlhoSequence& s, Tags & t):weight_tag(s, t),
        tablo_tag(s, t), weight_stable(s, t), red_light(s, t),
        green_light(s, t), perim_in(s, t), perim_out(s, t), logging(s, t), current_card(t),
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

        current_card.tag_name = get_setting<QString>("current_card_tag", s);
        current_card.property_name = get_setting<QString>("current_card_prop", s);

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
