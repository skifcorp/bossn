#ifndef BASEWEIGHTERINSTANTIAION_H
#define BASEWEIGHTERINSTANTIAION_H

#include "weighter.h"
#include "cardstructs.h"

#include <QVariantMap>


#include "qxorm_pch.h"
#include <memory>

namespace boost  { namespace rdb { namespace mysql {
    class mysql_database;
} } }


struct WeighterConf : public QVariantMap
{
public:
    int material;
    QString weighter_name;
    QString platform_type;

    QSqlDatabase database;

    std::unique_ptr<boost::rdb::mysql::mysql_database> db;

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

    WeighterConf();

    WeighterConf(WeighterConf&& other);

    WeighterConf& operator=(WeighterConf&& other);

};

#endif // BASEWEIGHTERINSTANTIAION_H
