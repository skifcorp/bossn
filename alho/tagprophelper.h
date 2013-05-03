#ifndef PROPFUNCHELPER_H
#define PROPFUNCHELPER_H

#include <boost/mpl/if.hpp>

#include <QString>

#include <tags.h>


template <bool IsRef>
struct TagPropHelper
{
public:
    TagPropHelper(const QString& tn, const QString& fn, Tags & t ) :
        property_name(fn), tag_name_ref( tn ), tags(t)
    {

    }
    TagPropHelper(const QString& tn, Tags & t ) :
        tag_name_ref( tn ), tags(t)
    {

    }

    typename boost::mpl::if_c<IsRef, const QString&, QString>::type property_name;

    void setProperty( const QVariant& v )
    {
        tags[tag_name_ref]->setProperty( property_name, v );
    }

    QVariant property( ) const
    {
        return tags[tag_name_ref]->getProperty( property_name );
    }

private:
    const QString& tag_name_ref;

    Tags & tags;
};



#endif // PROPFUNCHELPER_H
