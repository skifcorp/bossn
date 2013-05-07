#include "rdb_pch.h"

#include "weighterconf.h"
#include "mainsequence.h"


WeighterConf::WeighterConf()
{

}

WeighterConf::WeighterConf(WeighterConf&& other) : QVariantMap(other)
{
    qSwap( material, other.material );
    qSwap( weighter_name, other.weighter_name);
    qSwap( platform_type, other.platform_type );
    qSwap( database, other.database );
    qSwap( db, other.db);
    qSwap( struct_conf, other.struct_conf );
}


WeighterConf& WeighterConf::operator=(WeighterConf&& other)
{
    QVariantMap::operator =(other);

    material      = std::move( other.material );
    weighter_name = std::move( other.weighter_name);
    platform_type = std::move( other.platform_type );
    database      = std::move( other.database );
    db            = std::move( other.db);
    struct_conf   = std::move( other.struct_conf );

    return *this;
}
