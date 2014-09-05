#ifndef MAINSEQUENCESETTINGS_H
#define MAINSEQUENCESETTINGS_H

#include <QString>
#include <QVariant>

#include "tagmethod.h"
#include "tagfunchelper.h"
#include "tagprophelper.h"
#include "alhosequence.h"
#include "tags.h"
#include "mifarereader.h"

#include <boost/mpl/if.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/int.hpp>

#include <type_traits>

#include <name_of/name_of.h>
#include <tools/numeric_tools.h>

#include <vector>
#include <algorithm>

using std::vector;

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


    template <TagMethodType Typ2 = Typ>
    CallableTagMethod( CallableTagMethod&& other,
                       typename std::enable_if<IsTagWithFunc<Typ2>::value>::type * v = 0 ) :
                            TagMethod<Typ>( std::move(other) ),
                            TagFuncHelper<true>( std::move(other), TagMethod<Typ>::tag_name, TagMethod<Typ>::method_name )
    {
        Q_UNUSED(v)
        //qDebug() << "move 1";
    }

    template <TagMethodType Typ2 = Typ >
    CallableTagMethod( CallableTagMethod&& other,
                       typename std::enable_if<!IsTagWithFunc<Typ2>::value>::type * v = 0) :
                            TagMethod<Typ>( std::move(other) ),
                            TagPropHelper<false>( std::move(other), TagMethod<Typ>::tag_name )
    {
        Q_UNUSED(v)
        //qDebug() << "move 2";
    }


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

    ReaderTagMethods( ReaderTagMethods&& other ) :
        name ( std::move( other.name ) ),
        do_on( std::move( other.do_on ), name ),
        do_off( std::move( other.do_off ), name ),
        activate_idle( std::move( other.activate_idle ), name ),
        host_coded_key( std::move( other.host_coded_key ), name ),
        do_auth( std::move( other.do_auth ), name ),
        write_block( std::move( other.write_block ), name ),
        read_block( std::move( other.read_block ), name ),
        do_sound( std::move( other.do_sound ), name )
    {

    }

};

struct CollectionOfReaderTagMethods
{
public:
    CollectionOfReaderTagMethods( AlhoSequence& s, Tags & t ) :
        alho_sequence_(s), tags_(t)
    {

    }

    template <class ... Args>
    auto all_readers_do_on( Args && ... args )
    {
        assertNotEmpty();

        bool ret = true;
        for ( auto & r : reader_tags ) {
            auto res = r.do_on.func( std::forward<Args>(args)...  );
            if ( !res.toBool()  ) {
                qWarning() << "failed to_on reader: " << r.name;
                ret = false;
            }
        }
        return ret;
    }

    template <class ... Args>
    auto all_readers_do_off( Args && ... args )
    {
        assertNotEmpty();

        bool ret = true;
        for ( auto & r : reader_tags ) {
            auto res = r.do_off.func( std::forward<Args>(args)...  );
            if ( !res.toBool() ) {
                qWarning() << "failed to_off reader: " << r.name;
                ret = false;
            }
        }
        return ret;
    }

    template <class ... Args>
    auto all_readers_activate_idle( Args && ... args )
    {
        assertNotEmpty();

        auto num = int{0};

        for ( auto & r : reader_tags ) {
            QVariant ret = r.activate_idle.func( std::forward<Args>(args)... );
            ActivateCardISO14443A act = ret.value<ActivateCardISO14443A>();


            if ( act.active() ) {
                return std::make_pair( act, num );
            }

            ++num;
        }

        return std::make_pair( ActivateCardISO14443A{}, -1 );
    }


    template <class ... Args>
    auto host_coded_key( int num, Args && ... args )
    {
        assertNotEmptyAndCheckNum(num);
        return reader_tags[num].host_coded_key.func( std::forward<Args>(args)... );
    }

    template <class ... Args>
    auto do_auth( int num, Args && ... args )
    {
        assertNotEmptyAndCheckNum(num);
        return reader_tags[num].do_auth.func( std::forward<Args>(args)... );
    }

    template <class ... Args>
    auto write_block ( int num , Args && ... args )
    {
        assertNotEmptyAndCheckNum(num);
        return reader_tags[num].write_block.func( std::forward<Args>(args)... );
    }

    template <class ... Args>
    auto read_block( int num , Args && ... args )
    {
        assertNotEmptyAndCheckNum(num);
        return reader_tags[num].read_block.func( std::forward<Args>(args)... );
    }

    template <class ... Args>
    auto do_sound( int num, Args && ... args )
    {
        assertNotEmptyAndCheckNum(num);
        return reader_tags[num].do_sound.func( std::forward<Args>(args)... );
    }


    void createTag( const QString& tag_name, const QString& do_on,
                                             const QString& do_off,
                                             const QString& activate_idle,
                                             const QString& host_coded_key,
                                             const QString& do_auth,
                                             const QString& write_block,
                                             const QString& read_block,
                                             const QString& do_sound )
    {
        ReaderTagMethods reader(alho_sequence_, tags_);



        reader.name                     = tag_name;
        reader.do_on.func_name          = do_on;
        reader.do_off.func_name         = do_off;
        reader.activate_idle.func_name  = activate_idle;
        reader.host_coded_key.func_name = host_coded_key;
        reader.do_auth.func_name        = do_auth;
        reader.write_block.func_name    = write_block;
        reader.read_block.func_name     = read_block;
        reader.do_sound.func_name       = do_sound;

        reader_tags.push_back( std::move( reader ) );
    }

    ReaderTagMethods& operator[]( int num )
    {
        assertNotEmptyAndCheckNum(num);
        return reader_tags[num];
    }

private:
    AlhoSequence& alho_sequence_;
    Tags & tags_;

    vector<ReaderTagMethods> reader_tags;
    void assertNotEmpty() const
    {
        if ( reader_tags.empty() ) {
            qWarning() << "reader tags is empty";
            qFatal("Exiting");
        }
    }

    void assertNotEmptyAndCheckNum( int num )
    {
        assertNotEmpty();

        if ( num >= static_cast<int>(reader_tags.size()) || num < 0 ) {
            qWarning() << "passed num is " << num << " and size of array is: "<< reader_tags.size();
            qFatal("Exiting");
        }
    }

};

template <class T>
class CollectionOfDublicateCallableTags
{
public:
    CollectionOfDublicateCallableTags( AlhoSequence& as, Tags & t ): alho_sequence_(as), tags_(t)
    {

    }

    template <class ... Args>
    void func( Args && ... args )
    {
        for ( auto& t : tags )  {
            t.func( std::forward<Args>(args)... );
        }
    }


    auto find( const QString& tag_name )
    {
        return std::find_if( tags.begin(), tags.end(), [&]( const T& t ){
            return t.tag_name == tag_name;
        } );
    }


    void createTag( const QString& tag_name, const QString& method_name )
    {
        T t(alho_sequence_, tags_);
        t.tag_name = tag_name;
        t.method_name = method_name;

        tags.push_back( std::move( t ) );
    }

    bool containsTag ( const QString& tag_name )
    {
        return std::any_of( tags.begin(), tags.end(), [&]( const T& t ){
            return t.tag_name == tag_name;
        } );
    }

    auto begin()
    {
        return tags.begin();
    }
    auto end()
    {
        return tags.end();
    }
private:
    vector<T> tags;
    AlhoSequence& alho_sequence_;
    Tags & tags_;
};

template <template <TagMethodType> class TagMethodTyp, class ReaderMethodsType>
struct BaseMainSequenceSettings
{   
    using TabloTagType = TagMethodTyp<TagMethodType::Simple>;

    TagMethodTyp<TagMethodType::Simple> weight_tag;
    CollectionOfDublicateCallableTags<TabloTagType> tablo_tag;

    TagMethodTyp<TagMethodType::Simple> weight_stable;
    TagMethodTyp<TagMethodType::CompareValue> red_light;
    TagMethodTyp<TagMethodType::CompareValue> green_light;
    TagMethodTyp<TagMethodType::CompareValue> perim_in;
    TagMethodTyp<TagMethodType::CompareValue> perim_out;
    TagMethodTyp<TagMethodType::Simple> logging;
    TagMethodTyp<TagMethodType::Property> current_card;

    CollectionOfReaderTagMethods reader;

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

        //tablo_tag.tag_name = get_setting<QString>("tablo_tag", s);
        //tablo_tag.method_name = get_setting<QString>("tablo_method", s);

        QVariantList tablo_tags = get_setting<QVariantList>("tablo_tags", s);

        for ( const auto& v : tablo_tags ) {
            const auto& m = v.toMap();

            tablo_tag.createTag( get_setting<QString>( "tablo_tag", m ),
                                  get_setting<QString>( "tablo_method", m ) );
        }

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


/*
        reader.name                     = get_setting<QString>("reader_tag", s);
        reader.do_on.func_name          = get_setting<QString>("reader_do_on", s);
        reader.do_off.func_name         = get_setting<QString>("reader_do_off", s);
        reader.activate_idle.func_name  = get_setting<QString>("reader_activate_idle", s);
        reader.host_coded_key.func_name = get_setting<QString>("reader_host_coded_key", s);
        reader.do_auth.func_name        = get_setting<QString>("reader_do_auth", s);
        reader.write_block.func_name    = get_setting<QString>("reader_write_block", s);
        reader.read_block.func_name     = get_setting<QString>("reader_read_block", s);
        reader.do_sound.func_name       = get_setting<QString>("reader_sound", s);
*/
        QVariantList reader_tags = get_setting<QVariantList>("reader_tags", s);
        for ( const auto& v : reader_tags ) {
            const auto& m = v.toMap();

            reader.createTag(get_setting<QString>("reader_tag", m),
                             get_setting<QString>("reader_do_on", m),
                             get_setting<QString>("reader_do_off", m),
                             get_setting<QString>("reader_activate_idle", m),
                             get_setting<QString>("reader_host_coded_key", m),
                             get_setting<QString>("reader_do_auth", m),
                             get_setting<QString>("reader_write_block", m),
                             get_setting<QString>("reader_read_block", m),
                             get_setting<QString>("reader_sound", m) );
        }

    }
};

typedef BaseMainSequenceSettings<CallableTagMethod, ReaderTagMethods> MainSequenceSettings;

#endif // MAINSEQUENCESETTINGS_H
