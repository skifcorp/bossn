#ifndef PROTDB_H
#define PROTDB_H


#include <QxOrm.h>
#include <QxMemLeak.h>


#include <QString>


struct prot_conf
{
    QString NameDB;
    QString Names;
    QString NameVar;
    QString Parametrs;


    prot_conf() {}

    virtual ~prot_conf() {}
};


struct db_names
{
    QString DB_name;
    QString DB_r_name;

    db_names() {}
    virtual ~db_names() {}

};


struct prot_values
{
    QDateTime   time;
    float       value;

    prot_values() :value(0.0f) {}
    prot_values(const QDateTime& dt, float v) : time(dt), value(v){}
    virtual ~prot_values(){}

};



QX_REGISTER_PRIMARY_KEY(prot_values, QDateTime)


QX_REGISTER_HPP_EXPORT_DLL(prot_conf          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(db_names           , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(prot_values        , qx::trait::no_base_class_defined, 1)

#endif // PROTDB_H
