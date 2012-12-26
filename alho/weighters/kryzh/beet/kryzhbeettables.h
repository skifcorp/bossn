#ifndef KRYZHBEETTABLES_H
#define KRYZHBEETTABLES_H

#include <boost/rdb/mysql.hpp>
#include <boost/rdb/mysql_database.hpp>
#include <boost/rdb/mysql/datetime.hpp>


BOOST_RDB_BEGIN_TABLE(t_ttn_beet_table)
    BOOST_RDB_COLUMN(num_nakl     ,integer     )  //not null
    BOOST_RDB_COLUMN(date_time    ,datetime    )  //not null
    BOOST_RDB_COLUMN(car          ,integer     )  //not null
    BOOST_RDB_COLUMN(field        ,integer     )  //not null
    BOOST_RDB_COLUMN(real_field   ,integer     )
    BOOST_RDB_COLUMN(loader       ,integer     )
    BOOST_RDB_COLUMN(dt_of_load   ,datetime    )
    BOOST_RDB_COLUMN(driver       ,integer     )  //not null
    BOOST_RDB_COLUMN(brutto       ,integer     )  //default 0
    BOOST_RDB_COLUMN(dt_of_brutto ,datetime    )
    BOOST_RDB_COLUMN(tara         ,integer     )  //default 0
    BOOST_RDB_COLUMN(dt_of_tara   ,datetime    )
    BOOST_RDB_COLUMN(bum          ,integer     )
    BOOST_RDB_COLUMN(real_bum     ,integer     )
    BOOST_RDB_COLUMN(kagat         ,integer    )
    BOOST_RDB_COLUMN(dt_of_unload  ,datetime   )
    BOOST_RDB_COLUMN(routed_to_lab ,boolean    )
    BOOST_RDB_COLUMN(was_in_lab    ,boolean    )
    BOOST_RDB_COLUMN(num_kart      ,varchar<20>     )
    BOOST_RDB_COLUMN(copy          ,boolean    )
    BOOST_RDB_COLUMN(time_return   ,datetime   )  //not null
    BOOST_RDB_COLUMN(trailer          ,boolean )
    BOOST_RDB_COLUMN(time_of_brutto   ,varchar<8>  )
    BOOST_RDB_COLUMN(time_of_tara     ,varchar<8>  )
    BOOST_RDB_COLUMN(time_of_return   ,varchar<8>  )
    BOOST_RDB_COLUMN(real_loader      ,integer )
    BOOST_RDB_COLUMN(err              ,integer )
    BOOST_RDB_COLUMN(bum_platforma    ,integer )
    BOOST_RDB_COLUMN(rup_tara         ,integer )
    BOOST_RDB_COLUMN(real_rup_tara    ,integer )
    BOOST_RDB_COLUMN(brutto_platforma ,integer )
    BOOST_RDB_COLUMN(tara_platforma   ,integer )
    BOOST_RDB_COLUMN(field_from_car   ,integer )
BOOST_RDB_END_TABLE(t_ttn_beet_table)




BOOST_RDB_BEGIN_TABLE(t_cars_beet_table)
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
    BOOST_RDB_COLUMN(smena                        ,varchar<20>)
BOOST_RDB_END_TABLE(t_cars_beet_table)

#endif // KRYZHBEETTABLES_H
