#include "protdb.h"



QX_REGISTER_CPP_EXPORT_DLL(prot_conf)
QX_REGISTER_CPP_EXPORT_DLL(db_names)
QX_REGISTER_CPP_EXPORT_DLL(prot_values)




namespace qx {
    template <> void register_class(QxClass<prot_conf> & t)
    {
        t.data(&prot_conf::NameDB, "NameDB");
        t.data(&prot_conf::Names, "Names");
        t.data(&prot_conf::NameVar, "NameVar");
        t.data(&prot_conf::Parametrs, "Parametrs" );
    }

    template <> void register_class(QxClass<db_names> & t)
    {
        t.data(&db_names::DB_name, "DB_name");
        t.data(&db_names::DB_r_name, "DB_r_name");

    }

    template <> void register_class(QxClass<prot_values> & t)
    {
        t.id( &prot_values::time, "time");
        t.data(  &prot_values::value, "value" );
    }



}
