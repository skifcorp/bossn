#include "mriabeetdbstructs.h"
#include "cardstructs.h"
//#include "datetimehack.h"
//#include "precompiled.h"

#include "qxorm_pch.h"
#include "func.h"

//using namespace alho::mria::data;

QX_REGISTER_CPP_EXPORT_DLL(t_ttn_beet_mria)
QX_REGISTER_CPP_EXPORT_DLL(t_rez_beet_mria)
QX_REGISTER_CPP_EXPORT_DLL(t_def_field_beet_mria)
QX_REGISTER_CPP_EXPORT_DLL(t_bum_beet_mria)
QX_REGISTER_CPP_EXPORT_DLL(t_cars_beet_mria)
QX_REGISTER_CPP_EXPORT_DLL(t_kagat_beet_mria)
QX_REGISTER_CPP_EXPORT_DLL(t_kontr_beet_mria)
QX_REGISTER_CPP_EXPORT_DLL(t_field_beet_mria)
QX_REGISTER_CPP_EXPORT_DLL(t_const_beet_mria)
QX_REGISTER_CPP_EXPORT_DLL(t_bum_state_log_beet_mria)
QX_REGISTER_CPP_EXPORT_DLL(t_action_log_beet_mria)



t_ttn_beet_mria::t_ttn_beet_mria():num_nakl(-1), date_time(timeShitToDateTime(0)), car(0), field(0), real_field(0), loader(0), dt_of_load(timeShitToDateTime(0)),
                driver(0),  brutto(0), dt_of_brutto(timeShitToDateTime(0)),
                tara(0), dt_of_tara(timeShitToDateTime(0)), bum(0), real_bum(0), kagat(0), dt_of_unload(timeShitToDateTime(0)),
    routed_to_lab(0), was_in_lab(0), copy(1), time_return(timeShitToDateTime(0)),
               trailer(0), real_loader(0), err(0), bum_platforma(0), rup_tara(0), real_rup_tara(0),
    brutto_platforma(0), tara_platforma(0), field_from_car(0)
{

}

t_ttn_beet_mria::t_ttn_beet_mria(long id):num_nakl(id), date_time(timeShitToDateTime(0)), car(0), field(0), real_field(0), loader(0), dt_of_load(timeShitToDateTime(0)),
                driver(0),  brutto(0), dt_of_brutto(timeShitToDateTime(0)),
                tara(0), dt_of_tara(timeShitToDateTime(0)), bum(0), real_bum(0), kagat(0), dt_of_unload(timeShitToDateTime(0)),
    routed_to_lab(0), was_in_lab(0), copy(1), time_return(timeShitToDateTime(0)),
               trailer(0), real_loader(0), err(0), bum_platforma(0), rup_tara(0), real_rup_tara(0),
    brutto_platforma(0), tara_platforma(0), field_from_car(0)
{

}


namespace qx {
    template <> void register_class(QxClass<t_ttn_beet_mria> & t)
    {
        t.id  (&t_ttn_beet_mria::num_nakl, "num_nakl");
        t.data(&t_ttn_beet_mria::date_time, "date_time");
        t.data(&t_ttn_beet_mria::car, "car") ;
        t.data(&t_ttn_beet_mria::field , "field");
        t.data(&t_ttn_beet_mria::real_field , "real_field");
        t.data(&t_ttn_beet_mria::loader , "loader");
        t.data(&t_ttn_beet_mria::dt_of_load , "dt_of_load");
        t.data(&t_ttn_beet_mria::driver , "driver");
        t.data(&t_ttn_beet_mria::brutto , "brutto");
        t.data(&t_ttn_beet_mria::dt_of_brutto , "dt_of_brutto");
        t.data(&t_ttn_beet_mria::tara , "tara");
        t.data(&t_ttn_beet_mria::dt_of_tara , "dt_of_tara");
        t.data(&t_ttn_beet_mria::bum , "bum");
        t.data(&t_ttn_beet_mria::real_bum , "real_bum");
        t.data(&t_ttn_beet_mria::kagat , "kagat");
        t.data(&t_ttn_beet_mria::dt_of_unload , "dt_of_unload");
        t.data(&t_ttn_beet_mria::routed_to_lab , "routed_to_lab");
        t.data(&t_ttn_beet_mria::was_in_lab , "was_in_lab");
        t.data(&t_ttn_beet_mria::num_kart , "num_kart");
        t.data(&t_ttn_beet_mria::copy , "copy");
        t.data(&t_ttn_beet_mria::time_return , "time_return");
        t.data(&t_ttn_beet_mria::trailer , "trailer");
        t.data(&t_ttn_beet_mria::time_of_brutto , "time_of_brutto");
        t.data(&t_ttn_beet_mria::time_of_tara , "time_of_tara");
        t.data(&t_ttn_beet_mria::time_of_return , "time_of_return");
        t.data(&t_ttn_beet_mria::real_loader , "real_loader");
        t.data(&t_ttn_beet_mria::err , "err");
        t.data(&t_ttn_beet_mria::bum_platforma , "bum_platforma");
        t.data(&t_ttn_beet_mria::rup_tara , "rup_tara");
        t.data(&t_ttn_beet_mria::real_rup_tara , "real_rup_tara");
        t.data(&t_ttn_beet_mria::brutto_platforma , "brutto_platforma");
        t.data(&t_ttn_beet_mria::tara_platforma , "tara_platforma");
        t.data(&t_ttn_beet_mria::field_from_car , "field_from_car");


    }

    template <> void register_class(QxClass<t_rez_beet_mria> & t)
    {
        t.id  (&t_rez_beet_mria::num_nakl, "num_nakl");
        t.data(&t_rez_beet_mria::field, "field");
        t.data(&t_rez_beet_mria::bum, "bum");
        t.data(&t_rez_beet_mria::kagat, "kagat");
    }

    template <> void register_class(QxClass<t_def_field_beet_mria> & t)
    {
        t.id  (&t_def_field_beet_mria::id, "id");

        t.data(&t_def_field_beet_mria::num_field , "num_field");
        t.data(&t_def_field_beet_mria::num_loader , "num_loader");

    }

    template <> void register_class(QxClass<t_bum_beet_mria> & t)
    {
        t.id  (&t_bum_beet_mria::id , "id");

        t.data(&t_bum_beet_mria::state , "state");
        t.data(&t_bum_beet_mria::queue , "queue");
        t.data(&t_bum_beet_mria::kvoday, "kvoday");
    }


    template <> void register_class(QxClass<t_cars_beet_mria> & t)
    {
        t.id  (&t_cars_beet_mria::id , "id");

        t.data(&t_cars_beet_mria::name , "name");
        t.data(&t_cars_beet_mria::dump_body_truck , "dump_body_truck");
        t.data(&t_cars_beet_mria::side_board , "side_board");
        t.data(&t_cars_beet_mria::back_board , "back_board");
        t.data(&t_cars_beet_mria::mark , "mark");
        t.data(&t_cars_beet_mria::tara , "tara");
        t.data(&t_cars_beet_mria::driver1 , "driver1");
        t.data(&t_cars_beet_mria::driver2 , "driver2");
        t.data(&t_cars_beet_mria::atp_id , "atp_id");
        t.data(&t_cars_beet_mria::atp_name , "atp_name");
        t.data(&t_cars_beet_mria::num_field , "num_field");
        t.data(&t_cars_beet_mria::num_loader , "num_loader");
        t.data(&t_cars_beet_mria::trailer , "trailer");
        t.data(&t_cars_beet_mria::speed , "speed");
        t.data(&t_cars_beet_mria::brutto , "brutto");
        t.data(&t_cars_beet_mria::block , "block");
        t.data(&t_cars_beet_mria::start_time , "start_time");
        t.data(&t_cars_beet_mria::amount_of_car_for_middle_tara , "amount_of_car_for_middle_tara");
        t.data(&t_cars_beet_mria::vremja_na_hodku , "vremja_na_hodku");

        t.data(&t_cars_beet_mria::copy, "copy");
        t.data(&t_cars_beet_mria::fl_perimetr, "fl_perimetr");
        t.data(&t_cars_beet_mria::smena, "smena");

    }

    template <> void register_class(QxClass<t_kagat_beet_mria> & t)
    {
        t.id  (&t_kagat_beet_mria::id , "id");

        t.data(&t_kagat_beet_mria::state , "state");
    }

    template <> void register_class(QxClass<t_kontr_beet_mria> & t)
    {
        t.id  (&t_kontr_beet_mria::id , "id");

        t.data(&t_kontr_beet_mria::name , "name");
        t.data(&t_kontr_beet_mria::area , "area");
        t.data(&t_kontr_beet_mria::settlement , "settlement");
        t.data(&t_kontr_beet_mria::period , "period");
        t.data(&t_kontr_beet_mria::car_in_period , "car_in_period");
        t.data(&t_kontr_beet_mria::type , "type");
    }




    template <> void register_class(QxClass<t_field_beet_mria> & t)
    {
        t.id  (&t_field_beet_mria::id , "id");

        t.data(&t_field_beet_mria::distance , "distance");
        t.data(&t_field_beet_mria::info , "info");
    }


    template <> void register_class(QxClass<t_const_beet_mria> & t)
    {
        t.id  (&t_const_beet_mria::id , "id");

        t.data(&t_const_beet_mria::value , "value");
    }

    template <> void register_class(QxClass<t_bum_state_log_beet_mria> & t)
    {
        t.id  (&t_bum_state_log_beet_mria::id , "id");

        t.data(&t_bum_state_log_beet_mria::date_time , "date_time");
        t.data(&t_bum_state_log_beet_mria::bum , "bum");
        t.data(&t_bum_state_log_beet_mria::state , "state");
    }

    template <> void register_class(QxClass<t_action_log_beet_mria> & t)
    {
        t.id  (&t_action_log_beet_mria::id , "id");

        t.data(&t_action_log_beet_mria::dt , "dt");
        t.data(&t_action_log_beet_mria::event , "event");
        t.data(&t_action_log_beet_mria::info , "info");
        t.data(&t_action_log_beet_mria::time , "time");
        t.data(&t_action_log_beet_mria::type , "type");
        t.data(&t_action_log_beet_mria::ind , "ind");
    }

}


 
