#include "kryzhbeetdbstructs.h"
#include "cardstructs.h"
//#include "datetimehack.h"
//#include "precompiled.h"

#include "qxorm_pch.h"
#include "func.h"

QX_REGISTER_CPP_EXPORT_DLL(t_ttn_beet)
QX_REGISTER_CPP_EXPORT_DLL(t_rez_beet)
QX_REGISTER_CPP_EXPORT_DLL(t_def_field_beet)
QX_REGISTER_CPP_EXPORT_DLL(t_bum_beet)
QX_REGISTER_CPP_EXPORT_DLL(t_cars_beet)
QX_REGISTER_CPP_EXPORT_DLL(t_kagat_beet)
QX_REGISTER_CPP_EXPORT_DLL(t_kontr_beet)
QX_REGISTER_CPP_EXPORT_DLL(t_field_beet)
QX_REGISTER_CPP_EXPORT_DLL(t_const_beet)
QX_REGISTER_CPP_EXPORT_DLL(t_bum_state_log_beet)
QX_REGISTER_CPP_EXPORT_DLL(t_action_log_beet)

t_ttn_beet::t_ttn_beet():num_nakl(-1), date_time(timeShitToDateTime(0)), car(0), field(0), real_field(0), loader(0), dt_of_load(timeShitToDateTime(0)),
                driver(0),  brutto(0), dt_of_brutto(timeShitToDateTime(0)),
                tara(0), dt_of_tara(timeShitToDateTime(0)), bum(0), real_bum(0), kagat(0), dt_of_unload(timeShitToDateTime(0)),
    routed_to_lab(0), was_in_lab(0), copy(1), time_return(timeShitToDateTime(0)),
               trailer(0), real_loader(0), err(0), bum_platforma(0), rup_tara(0), real_rup_tara(0),
    brutto_platforma(0), tara_platforma(0), field_from_car(0)
{

}

t_ttn_beet::t_ttn_beet(long id):num_nakl(id), date_time(timeShitToDateTime(0)), car(0), field(0), real_field(0), loader(0), dt_of_load(timeShitToDateTime(0)),
                driver(0),  brutto(0), dt_of_brutto(timeShitToDateTime(0)),
                tara(0), dt_of_tara(timeShitToDateTime(0)), bum(0), real_bum(0), kagat(0), dt_of_unload(timeShitToDateTime(0)),
    routed_to_lab(0), was_in_lab(0), copy(1), time_return(timeShitToDateTime(0)),
               trailer(0), real_loader(0), err(0), bum_platforma(0), rup_tara(0), real_rup_tara(0),
    brutto_platforma(0), tara_platforma(0), field_from_car(0)
{

}


namespace qx {
    template <> void register_class(QxClass<t_ttn_beet> & t)
    {
        t.id  (&t_ttn_beet::num_nakl, "num_nakl");
        t.data(&t_ttn_beet::date_time, "date_time");
        t.data(&t_ttn_beet::car, "car") ;
        t.data(&t_ttn_beet::field , "field");
        t.data(&t_ttn_beet::real_field , "real_field");
        t.data(&t_ttn_beet::loader , "loader");
        t.data(&t_ttn_beet::dt_of_load , "dt_of_load");
        t.data(&t_ttn_beet::driver , "driver");
        t.data(&t_ttn_beet::brutto , "brutto");
        t.data(&t_ttn_beet::dt_of_brutto , "dt_of_brutto");
        t.data(&t_ttn_beet::tara , "tara");
        t.data(&t_ttn_beet::dt_of_tara , "dt_of_tara");
        t.data(&t_ttn_beet::bum , "bum");
        t.data(&t_ttn_beet::real_bum , "real_bum");
        t.data(&t_ttn_beet::kagat , "kagat");
        t.data(&t_ttn_beet::dt_of_unload , "dt_of_unload");
        t.data(&t_ttn_beet::routed_to_lab , "routed_to_lab");
        t.data(&t_ttn_beet::was_in_lab , "was_in_lab");
        t.data(&t_ttn_beet::num_kart , "num_kart");
        t.data(&t_ttn_beet::copy , "copy");
        t.data(&t_ttn_beet::time_return , "time_return");
        t.data(&t_ttn_beet::trailer , "trailer");
        t.data(&t_ttn_beet::time_of_brutto , "time_of_brutto");
        t.data(&t_ttn_beet::time_of_tara , "time_of_tara");
        t.data(&t_ttn_beet::time_of_return , "time_of_return");
        t.data(&t_ttn_beet::real_loader , "real_loader");
        t.data(&t_ttn_beet::err , "err");
        t.data(&t_ttn_beet::bum_platforma , "bum_platforma");
        t.data(&t_ttn_beet::rup_tara , "rup_tara");
        t.data(&t_ttn_beet::real_rup_tara , "real_rup_tara");
        t.data(&t_ttn_beet::brutto_platforma , "brutto_platforma");
        t.data(&t_ttn_beet::tara_platforma , "tara_platforma");
        t.data(&t_ttn_beet::field_from_car , "field_from_car");


    }

    template <> void register_class(QxClass<t_rez_beet> & t)
    {
        t.id  (&t_rez_beet::num_nakl, "num_nakl");
        t.data(&t_rez_beet::field, "field");
        t.data(&t_rez_beet::bum, "bum");
        t.data(&t_rez_beet::kagat, "kagat");
    }

    template <> void register_class(QxClass<t_def_field_beet> & t)
    {
        t.id  (&t_def_field_beet::id, "id");

        t.data(&t_def_field_beet::num_field , "num_field");
        t.data(&t_def_field_beet::num_loader , "num_loader");

    }

    template <> void register_class(QxClass<t_bum_beet> & t)
    {
        t.id  (&t_bum_beet::id , "id");

        t.data(&t_bum_beet::state , "state");
        t.data(&t_bum_beet::queue , "queue");
        t.data(&t_bum_beet::kvoday, "kvoday");
    }


    template <> void register_class(QxClass<t_cars_beet> & t)
    {
        t.id  (&t_cars_beet::id , "id");

        t.data(&t_cars_beet::name , "name");
        t.data(&t_cars_beet::dump_body_truck , "dump_body_truck");
        t.data(&t_cars_beet::side_board , "side_board");
        t.data(&t_cars_beet::back_board , "back_board");
        t.data(&t_cars_beet::mark , "mark");
        t.data(&t_cars_beet::tara , "tara");
        t.data(&t_cars_beet::driver1 , "driver1");
        t.data(&t_cars_beet::driver2 , "driver2");
        t.data(&t_cars_beet::atp_id , "atp_id");
        t.data(&t_cars_beet::atp_name , "atp_name");
        t.data(&t_cars_beet::num_field , "num_field");
        t.data(&t_cars_beet::num_loader , "num_loader");
        t.data(&t_cars_beet::trailer , "trailer");
        t.data(&t_cars_beet::speed , "speed");
        t.data(&t_cars_beet::brutto , "brutto");
        t.data(&t_cars_beet::block , "block");
        t.data(&t_cars_beet::start_time , "start_time");
        t.data(&t_cars_beet::amount_of_car_for_middle_tara , "amount_of_car_for_middle_tara");
        t.data(&t_cars_beet::vremja_na_hodku , "vremja_na_hodku");

        t.data(&t_cars_beet::smena, "smena");

    }

    template <> void register_class(QxClass<t_kagat_beet> & t)
    {
        t.id  (&t_kagat_beet::id , "id");

        t.data(&t_kagat_beet::state , "state");
    }

    template <> void register_class(QxClass<t_kontr_beet> & t)
    {
        t.id  (&t_kontr_beet::id , "id");

        t.data(&t_kontr_beet::name , "name");
        t.data(&t_kontr_beet::area , "area");
        t.data(&t_kontr_beet::settlement , "settlement");
        t.data(&t_kontr_beet::period , "period");
        t.data(&t_kontr_beet::car_in_period , "car_in_period");
        t.data(&t_kontr_beet::type , "type");
    }




    template <> void register_class(QxClass<t_field_beet> & t)
    {
        t.id  (&t_field_beet::id , "id");

        t.data(&t_field_beet::distance , "distance");
        t.data(&t_field_beet::info , "info");
    }


    template <> void register_class(QxClass<t_const_beet> & t)
    {
        t.id  (&t_const_beet::id , "id");

        t.data(&t_const_beet::value , "value");
    }

    template <> void register_class(QxClass<t_bum_state_log_beet> & t)
    {
        t.id  (&t_bum_state_log_beet::id , "id");

        t.data(&t_bum_state_log_beet::date_time , "date_time");
        t.data(&t_bum_state_log_beet::bum , "bum");
        t.data(&t_bum_state_log_beet::state , "state");
    }

    template <> void register_class(QxClass<t_action_log_beet> & t)
    {
        t.id  (&t_action_log_beet::id , "id");

        t.data(&t_action_log_beet::dt , "dt");
        t.data(&t_action_log_beet::event , "event");
        t.data(&t_action_log_beet::info , "info");
        t.data(&t_action_log_beet::time , "time");
        t.data(&t_action_log_beet::type , "type");
        t.data(&t_action_log_beet::ind , "ind");
    }

}
