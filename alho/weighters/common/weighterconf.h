#ifndef BASEWEIGHTERINSTANTIAION_H
#define BASEWEIGHTERINSTANTIAION_H

#include "weighter.h"
#include "cardstructs.h"

#include <QVariantMap>


#include "qxorm_pch.h"


struct WeighterConf : public QVariantMap
{
public:
    int material;
    QString weighter_name;
    QString platform_type;

    QSqlDatabase database;

    boost::rdb::mysql::mysql_database db;

    StructConf struct_conf;

    template <class T>
    T member(const QString& name) const
    {
        auto iter = find(name);
        if ( iter == end() ) {
            qWarning() << "cant find member: " << name << " in options for: "<<weighter_name;
            qFatal("exit");
        }

        return iter->value<T>();
    }

    WeighterConf() {}

    WeighterConf(WeighterConf&& other) : QVariantMap(other)
    {
        qSwap( material, other.material );
        qSwap( weighter_name, other.weighter_name);
        qSwap( platform_type, other.platform_type );
        qSwap( database, other.database );
        qSwap( db, other.db);
        qSwap( struct_conf, other.struct_conf );
    }

    WeighterConf& operator=(WeighterConf&& other)
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

};

#endif // BASEWEIGHTERINSTANTIAION_H
