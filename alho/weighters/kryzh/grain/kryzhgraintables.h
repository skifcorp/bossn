#ifndef KRYZHGRAINTABLES_H
#define KRYZHGRAINTABLES_H

#ifdef AAAA

namespace alho { namespace kryzh {

BOOST_RDB_BEGIN_TABLE(t_ttn_grain_table)
    BOOST_RDB_COLUMN(num_nakl, integer)
    BOOST_RDB_COLUMN(date_time, datetime)
    BOOST_RDB_COLUMN(car, integer)
    BOOST_RDB_COLUMN(field, integer)
    BOOST_RDB_COLUMN(real_field, integer)
    BOOST_RDB_COLUMN(loader, integer)
    BOOST_RDB_COLUMN(dt_of_load, datetime)
    BOOST_RDB_COLUMN(driver, integer)
    BOOST_RDB_COLUMN(brutto, integer)
    BOOST_RDB_COLUMN(dt_of_brutto, datetime)
    BOOST_RDB_COLUMN(tara, integer)
    BOOST_RDB_COLUMN(dt_of_tara, datetime)
    BOOST_RDB_COLUMN(bum, integer)
    BOOST_RDB_COLUMN(real_bum, integer)
    BOOST_RDB_COLUMN(kagat, integer)
    BOOST_RDB_COLUMN(dt_of_unload, datetime)
    BOOST_RDB_COLUMN(routed_to_lab, boolean)
    BOOST_RDB_COLUMN(was_in_lab, boolean)
    BOOST_RDB_COLUMN(num_kart, varchar<20>)
    BOOST_RDB_COLUMN(copy, boolean)
    BOOST_RDB_COLUMN(time_return, datetime)
    BOOST_RDB_COLUMN(trailer, boolean)
    BOOST_RDB_COLUMN(time_of_brutto, varchar<8>)
    BOOST_RDB_COLUMN(time_of_tara, varchar<8>)
    BOOST_RDB_COLUMN(time_of_return, varchar<8>)
    BOOST_RDB_COLUMN(real_loader, integer)
    BOOST_RDB_COLUMN(err, integer)
    BOOST_RDB_COLUMN(bum_platforma, integer)
    BOOST_RDB_COLUMN(rup_tara, integer)
    BOOST_RDB_COLUMN(real_rup_tara, integer)
    BOOST_RDB_COLUMN(brutto_platforma, integer)
    BOOST_RDB_COLUMN(tara_platforma, integer)
    BOOST_RDB_COLUMN(field_from_car, integer)
    BOOST_RDB_COLUMN(culture, integer)
    BOOST_RDB_COLUMN(sort, integer)
    BOOST_RDB_COLUMN(repr, integer )
    BOOST_RDB_COLUMN(culture_name, varchar<100>)
BOOST_RDB_END_TABLE(t_ttn_grain_table)





BOOST_RDB_BEGIN_TABLE(t_cars_grain_table)
    BOOST_RDB_COLUMN(id                           ,integer    )
    BOOST_RDB_COLUMN(name                         ,varchar<20>)
    BOOST_RDB_COLUMN(dump_body_truck              ,boolean    )
    BOOST_RDB_COLUMN(side_board                   ,boolean    )
    BOOST_RDB_COLUMN(back_board                   ,boolean    )
    BOOST_RDB_COLUMN(mark                         ,varchar<30>)
    BOOST_RDB_COLUMN(tara                         ,integer    )
    BOOST_RDB_COLUMN(driver1                      ,varchar<20>)
    BOOST_RDB_COLUMN(driver2                      ,varchar<20>)
    BOOST_RDB_COLUMN(atp_id                       ,integer    )
    BOOST_RDB_COLUMN(atp_name                     ,varchar<30>)
    BOOST_RDB_COLUMN(num_field                    ,integer    )
    BOOST_RDB_COLUMN(num_loader                   ,integer    )
    BOOST_RDB_COLUMN(trailer                      ,varchar<10>)
    BOOST_RDB_COLUMN(speed                        ,integer    )
    BOOST_RDB_COLUMN(brutto                       ,integer    )
    BOOST_RDB_COLUMN(block                        ,boolean    )
    BOOST_RDB_COLUMN(start_time                   ,datetime   )
    BOOST_RDB_COLUMN(amount_of_car_for_middle_tara,integer    )
    BOOST_RDB_COLUMN(vremja_na_hodku              ,integer    )
    BOOST_RDB_COLUMN(copy                         ,boolean    )
    BOOST_RDB_COLUMN(fl_perimetr                  ,boolean    )
    BOOST_RDB_COLUMN(smena                        ,varchar<20>)
BOOST_RDB_END_TABLE(t_cars_grain_table)




BOOST_RDB_BEGIN_TABLE(t_field_grain_table)
    BOOST_RDB_COLUMN(id                     ,integer      )
    BOOST_RDB_COLUMN(distance               ,integer      )
    BOOST_RDB_COLUMN(info                   ,varchar<100> )
BOOST_RDB_END_TABLE(t_field_grain_table)



BOOST_RDB_BEGIN_TABLE(t_bum_grain_table)
    BOOST_RDB_COLUMN(id, integer)
    BOOST_RDB_COLUMN(state, boolean)
    BOOST_RDB_COLUMN(queue, integer)
    BOOST_RDB_COLUMN(heavyweight, integer)
    BOOST_RDB_COLUMN(client, integer)
    BOOST_RDB_COLUMN(field, integer)
BOOST_RDB_END_TABLE(t_bum_grain_table)

BOOST_RDB_BEGIN_TABLE(t_kagat_grain_table)
    BOOST_RDB_COLUMN(id                 , integer)
    BOOST_RDB_COLUMN(state              , boolean)
BOOST_RDB_END_TABLE(t_kagat_grain_table)

BOOST_RDB_BEGIN_TABLE(t_kontr_grain_table)
    BOOST_RDB_COLUMN(id           , integer)
    BOOST_RDB_COLUMN(name         , varchar<50>)
    BOOST_RDB_COLUMN(area         , varchar<50>)
    BOOST_RDB_COLUMN(settlement   , varchar<50>)
    BOOST_RDB_COLUMN(period       , integer)
    BOOST_RDB_COLUMN(car_in_period, integer)
    BOOST_RDB_COLUMN(type         , integer)
    BOOST_RDB_COLUMN(firstcar     , integer)
BOOST_RDB_END_TABLE(t_kontr_grain_table)

BOOST_RDB_BEGIN_TABLE(t_bum_state_log_grain_table)
    BOOST_RDB_COLUMN(id, integer)
    BOOST_RDB_COLUMN(date_time, datetime)
    BOOST_RDB_COLUMN(bum, integer)
    BOOST_RDB_COLUMN(state, boolean)
BOOST_RDB_END_TABLE(t_bum_state_log_grain_table)



BOOST_RDB_BEGIN_TABLE(t_prikaz_grain_table)
    BOOST_RDB_COLUMN(num_nakl, integer)
    BOOST_RDB_COLUMN(date_time, datetime)
    BOOST_RDB_COLUMN(lim, integer)
    BOOST_RDB_COLUMN(rest, integer)
    BOOST_RDB_COLUMN(place_out, integer)
    BOOST_RDB_COLUMN(place_in, integer)
    BOOST_RDB_COLUMN(copy, boolean)
    BOOST_RDB_COLUMN(culture, varchar<100>)
    BOOST_RDB_COLUMN(sort, integer)
    BOOST_RDB_COLUMN(repr, integer)
    BOOST_RDB_COLUMN(virtual_rest, integer)
BOOST_RDB_END_TABLE(t_prikaz_grain_table)

BOOST_RDB_BEGIN_TABLE(t_ttno_grain_table)
    BOOST_RDB_COLUMN(date_time, datetime)
    BOOST_RDB_COLUMN(num_nakl, integer)
    BOOST_RDB_COLUMN(car, integer)
    BOOST_RDB_COLUMN(prikaz, integer)
    BOOST_RDB_COLUMN(place_out, integer)
    BOOST_RDB_COLUMN(place_in, integer)
    BOOST_RDB_COLUMN(driver, integer)
    BOOST_RDB_COLUMN(brutto, integer)
    BOOST_RDB_COLUMN(dt_of_brutto, datetime)
    BOOST_RDB_COLUMN(tara, integer)
    BOOST_RDB_COLUMN(dt_of_tara, datetime)
    BOOST_RDB_COLUMN(num_kart, varchar<20>)
    BOOST_RDB_COLUMN(copy, boolean)
    BOOST_RDB_COLUMN(trailer, integer)
    BOOST_RDB_COLUMN(culture, varchar<100>)
    BOOST_RDB_COLUMN(sort, integer)
    BOOST_RDB_COLUMN(repr, integer)
BOOST_RDB_END_TABLE(t_ttno_grain_table)

} }

#endif

#endif // KRYZHGRAINTABLES_H
