#ifndef BASEWEIGHTERINSTANTIAION_H
#define BASEWEIGHTERINSTANTIAION_H

#include "weighter.h"
#include "cardstructs.h"

#include <QVariantMap>
class MainSequence;

struct WeighterConf : public QVariantMap
{
public:
    int material;
    QString weighter_name;
    QString platform_type;

    QSqlDatabase database;

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
};

#endif // BASEWEIGHTERINSTANTIAION_H
