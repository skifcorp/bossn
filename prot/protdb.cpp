#include "protdb.h"

#if 0

QX_REGISTER_CPP_EXPORT_DLL(prot_conf)
QX_REGISTER_CPP_EXPORT_DLL(db_names)
QX_REGISTER_CPP_EXPORT_DLL(prot_values)
QX_REGISTER_CPP_EXPORT_DLL(prot_work)
QX_REGISTER_CPP_EXPORT_DLL(message_log)
QX_REGISTER_CPP_EXPORT_DLL(prot_value_scale)

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

    template <> void register_class(QxClass<prot_work> & t)
    {
        t.id  (  &prot_work::start_from, "start_from" );
        t.data(  &prot_work::work_till , "work_till"  );
    }

    template <> void register_class(QxClass<message_log> & t)
    {
        t.id  ( &message_log::num, "msg_num" );

        t.data( &message_log::sender_id, "sender_id"  );
        t.data( &message_log::type, "type" );
        t.data( &message_log::when, "msg_time" );
        t.data( &message_log::message, "message" );
    }

    template <> void register_class(QxClass<prot_value_scale> & t)
    {
        t.id  ( &prot_value_scale::time, "time" );

        t.data( &prot_value_scale::min, "min" );
        t.data( &prot_value_scale::max, "max" );
    }
}
#endif
