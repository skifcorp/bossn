#include "dbstructs.h"
#include "cardstructs.h"
//#include "datetimehack.h"
//#include "precompiled.h"
#include <QxOrm.h>
#include <QxMemLeak.h>
#include "func.h"

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

t_ttn::t_ttn():num_nakl(-1), date_time(timeShitToDateTime(0)), car(0), field(0), real_field(0), loader(0), dt_of_load(timeShitToDateTime(0)),
                driver(0),  brutto(0), dt_of_brutto(timeShitToDateTime(0)),
                tara(0), dt_of_tara(timeShitToDateTime(0)), bum(0), real_bum(0), kagat(0), dt_of_unload(timeShitToDateTime(0)),
    routed_to_lab(0), was_in_lab(0), copy(0), time_return(timeShitToDateTime(0)),
               trailer(0), real_loader(0), err(0), bum_platforma(0), rup_tara(0), real_rup_tara(0),
    brutto_platforma(0), tara_platforma(0), field_from_car(0), culture(0),sort(0),repr(0)
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
        t.data(&t_ttn::culture , "culture");
        t.data(&t_ttn::sort , "sort");
        t.data(&t_ttn::repr , "repr");


    }

    template <> void register_class(QxClass<t_rez> & t)
    {
        t.id  (&t_rez::num_nakl, "num_nakl");
        t.data(&t_rez::field, "field");
        t.data(&t_rez::bum, "bum");
        t.data(&t_rez::kagat, "kagat");
    }

    template <> void register_class(QxClass<t_def_field> & t)
    {
        t.id  (&t_def_field::id, "id");

        t.data(&t_def_field::num_field , "num_field");
        t.data(&t_def_field::num_loader , "num_loader");

    }

    template <> void register_class(QxClass<t_bum> & t)
    {
        t.id  (&t_bum::id , "id");

        t.data(&t_bum::state , "state");
        t.data(&t_bum::queue , "queue");
    }


    template <> void register_class(QxClass<t_cars> & t)
    {
        t.id  (&t_cars::id , "id");

        t.data(&t_cars::name , "name");
        t.data(&t_cars::dump_body_truck , "dump_body_truck");
        t.data(&t_cars::side_board , "side_board");
        t.data(&t_cars::back_board , "back_board");
        t.data(&t_cars::mark , "mark");
        t.data(&t_cars::tara , "tara");
        t.data(&t_cars::driver1 , "driver1");
        t.data(&t_cars::driver2 , "driver2");
        t.data(&t_cars::atp_id , "atp_id");
        t.data(&t_cars::atp_name , "atp_name");
        t.data(&t_cars::num_field , "num_field");
        t.data(&t_cars::num_loader , "num_loader");
        t.data(&t_cars::trailer , "trailer");
        t.data(&t_cars::speed , "speed");
        t.data(&t_cars::brutto , "brutto");
        t.data(&t_cars::block , "block");
        t.data(&t_cars::start_time , "start_time");
        t.data(&t_cars::amount_of_car_for_middle_tara , "amount_of_car_for_middle_tara");
        t.data(&t_cars::vremja_na_hodku , "vremja_na_hodku");
    }

    template <> void register_class(QxClass<t_kagat> & t)
    {
        t.id  (&t_kagat::id , "id");

        t.data(&t_kagat::state , "state");
    }

    template <> void register_class(QxClass<t_kontr> & t)
    {
        t.id  (&t_kontr::id , "id");

        t.data(&t_kontr::name , "name");
        t.data(&t_kontr::area , "area");
        t.data(&t_kontr::settlement , "settlement");
        t.data(&t_kontr::period , "period");
        t.data(&t_kontr::car_in_period , "car_in_period");
        t.data(&t_kontr::type , "type");
    }




    template <> void register_class(QxClass<t_field> & t)
    {
        t.id  (&t_field::id , "id");

        t.data(&t_field::distance , "distance");
        t.data(&t_field::info , "info");
    }


    template <> void register_class(QxClass<t_const> & t)
    {
        t.id  (&t_const::id , "id");

        t.data(&t_const::value , "value");
    }

    template <> void register_class(QxClass<t_bum_state_log> & t)
    {
        t.id  (&t_bum_state_log::id , "id");

        t.data(&t_bum_state_log::date_time , "date_time");
        t.data(&t_bum_state_log::bum , "bum");
        t.data(&t_bum_state_log::state , "state");
    }

    template <> void register_class(QxClass<t_action_log> & t)
    {
        t.id  (&t_action_log::id , "id");

        t.data(&t_action_log::dt , "dt");
        t.data(&t_action_log::event , "event");
        t.data(&t_action_log::info , "info");
        t.data(&t_action_log::time , "time");
        t.data(&t_action_log::type , "type");
        t.data(&t_action_log::ind , "ind");
    }

}
