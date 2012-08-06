#ifndef BASEWEIGHTERINSTANTIAION_H
#define BASEWEIGHTERINSTANTIAION_H

#include "baseweighter.h"
//#include "mainsequence.h"
#include "cardstructs.h"

#include <QVariantMap>
class MainSequence;

struct WeighterConf : public QVariantMap
{
public:
    int material;
    QString weighter_name;


    QSqlDatabase database;

    StructConf struct_conf;

    BaseWeighter::Pointer createWeighter (MainSequence& seq);

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

    QString platform_type;
};

#endif // BASEWEIGHTERINSTANTIAION_H
