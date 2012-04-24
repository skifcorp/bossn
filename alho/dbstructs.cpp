#include "dbstructs.h"
#include "datetimehack.h"
//#include "precompiled.h"
#include <QxOrm.h>
#include <QxMemLeak.h>

QX_REGISTER_CPP_EXPORT_DLL(t_ttn)
QX_REGISTER_CPP_EXPORT_DLL(t_rez)
QX_REGISTER_CPP_EXPORT_DLL(t_def_field)
QX_REGISTER_CPP_EXPORT_DLL(t_bum)
QX_REGISTER_CPP_EXPORT_DLL(t_cars)
QX_REGISTER_CPP_EXPORT_DLL(t_kagat)
QX_REGISTER_CPP_EXPORT_DLL(t_kontr)
QX_REGISTER_CPP_EXPORT_DLL(t_field)
QX_REGISTER_CPP_EXPORT_DLL(t_const)
QX_REGISTER_CPP_EXPORT_DLL(t_bum_state_log)
QX_REGISTER_CPP_EXPORT_DLL(t_action_log)

t_ttn::t_ttn():num_nakl(0), date_time(zero_date_time()), car(0), field(0), real_field(0), loader(0), dt_of_load(zero_date_time()),
                driver(0),  brutto(0), dt_of_brutto(zero_date_time()),
                tara(0), dt_of_tara(zero_date_time()), bum(0), real_bum(0), kagat(0), dt_of_unload(zero_date_time()),
    routed_to_lab(0), was_in_lab(0), copy(0), time_return(zero_date_time()),
               trailer(0), real_loader(0), err(0), bum_platforma(0), rup_tara(0), real_rup_tara(0),
                brutto_platforma(0), tara_platforma(0), field_from_car(0)
{

}

namespace qx {
    template <> void register_class(QxClass<t_ttn> & t)
    {
        t.id  (&t_ttn::num_nakl, "num_nakl");
        t.data(&t_ttn::date_time, "date_time");
        t.data(&t_ttn::car, "car") ;
        t.data(&t_ttn::field , "field");
        t.data(&t_ttn::real_field , "real_field");
        t.data(&t_ttn::loader , "loader");
        t.data(&t_ttn::dt_of_load , "dt_of_load");
        t.data(&t_ttn::driver , "driver");
        t.data(&t_ttn::brutto , "brutto");
        t.data(&t_ttn::dt_of_brutto , "dt_of_brutto");
        t.data(&t_ttn::tara , "tara");
        t.data(&t_ttn::dt_of_tara , "dt_of_tara");
        t.data(&t_ttn::bum , "bum");
        t.data(&t_ttn::real_bum , "real_bum");
        t.data(&t_ttn::kagat , "kagat");
        t.data(&t_ttn::dt_of_unload , "dt_of_unload");
        t.data(&t_ttn::routed_to_lab , "routed_to_lab");
        t.data(&t_ttn::was_in_lab , "was_in_lab");
        t.data(&t_ttn::num_kart , "num_kart");
        t.data(&t_ttn::copy , "copy");
        t.data(&t_ttn::time_return , "time_return");
        t.data(&t_ttn::trailer , "trailer");
        t.data(&t_ttn::time_of_brutto , "time_of_brutto");
        t.data(&t_ttn::time_of_tara , "time_of_tara");
        t.data(&t_ttn::time_of_return , "time_of_return");
        t.data(&t_ttn::real_loader , "real_loader");
        t.data(&t_ttn::err , "err");
        t.data(&t_ttn::bum_platforma , "bum_platforma");
        t.data(&t_ttn::rup_tara , "rup_tara");
        t.data(&t_ttn::real_rup_tara , "real_rup_tara");
        t.data(&t_ttn::brutto_platforma , "brutto_platforma");
        t.data(&t_ttn::tara_platforma , "tara_platforma");
        t.data(&t_ttn::field_from_car , "field_from_car");
    }

    template <> void register_class(QxClass<t_rez> & t)
    {
    }

    template <> void register_class(QxClass<t_def_field> & t)
    {
    }

    template <> void register_class(QxClass<t_bum> & t)
    {
    }


    template <> void register_class(QxClass<t_cars> & t)
    {
    }

    template <> void register_class(QxClass<t_kagat> & t)
    {
    }

    template <> void register_class(QxClass<t_kontr> & t)
    {
    }




    template <> void register_class(QxClass<t_field> & t)
    {
    }


    template <> void register_class(QxClass<t_const> & t)
    {
    }

    template <> void register_class(QxClass<t_bum_state_log> & t)
    {
    }

    template <> void register_class(QxClass<t_action_log> & t)
    {
    }

}
