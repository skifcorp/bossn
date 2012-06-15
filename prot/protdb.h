#ifndef PROTDB_H
#define PROTDB_H


#include <QxOrm.h>
#include <QxMemLeak.h>


#include <QString>

#if 0
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
};


struct tag_prot
{
    QDateTime   time;
    float       value;
};

QX_REGISTER_HPP_EXPORT_DLL(prot_conf          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(db_names           , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(tag_prot           , qx::trait::no_base_class_defined, 1)

#endif

#endif // PROTDB_H
